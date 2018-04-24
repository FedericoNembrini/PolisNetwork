using System;
using System.Text;
using System.Diagnostics;
using Windows.Storage.Streams;
using Windows.System.Threading;
using Windows.Networking.Sockets;
using Windows.ApplicationModel.Background;
using System.Runtime.InteropServices.WindowsRuntime;

namespace PolisNetworkHub_HTTPServer
{
    public sealed class StartupTask : IBackgroundTask
    {
        private static BackgroundTaskDeferral deffereal;
        private static string urlPublishLog = "http://polis.inno-school.org/polis/php/api/publishMetric.php";

        #region MainActivity
        public async void Run(IBackgroundTaskInstance taskInstance)
        {
            deffereal = taskInstance.GetDeferral();

            MyServer Server = new MyServer();
            
            await ThreadPool.RunAsync(workItem =>
             {
                 Server.Start();
             });
        }
        #endregion

        #region Server Declaration
        internal class MyServer
        {
            HTTPRequestHandler postData = new HTTPRequestHandler(urlPublishLog);
            private const uint BufferSize = 8192;

            public async void Start()
            {
                StreamSocketListener listener = new StreamSocketListener();

                await listener.BindServiceNameAsync("80");
                listener.ConnectionReceived += Listener_ConnectionReceived;
            }

            private async void Listener_ConnectionReceived(StreamSocketListener sender, StreamSocketListenerConnectionReceivedEventArgs args)
            {
                StringBuilder request = new StringBuilder();

                //Retrive Data
                using (var input = args.Socket.InputStream)
                {
                    var data = new byte[BufferSize];
                    IBuffer buffer = data.AsBuffer();
                    var dataRead = BufferSize;

                    while (dataRead == BufferSize)
                    {
                        await input.ReadAsync(buffer, BufferSize, InputStreamOptions.Partial);
                        request.Append(Encoding.UTF8.GetString(
                                                      data, 0, data.Length));
                        dataRead = buffer.Length;
                    }
                }

                string query = GetQueryString(request);
                
                //Response to the GET, Implement ONLY for Test with Browser
                /*using (var output = args.Socket.OutputStream)
                {
                    using (var response = output.AsStreamForWrite())
                    {
                        var html = Encoding.UTF8.GetBytes(
                        $"<html><head><title>Background Message</title></head><body>{query}</body></html>");
                        using (var bodyStream = new MemoryStream(html))
                        {
                            var header = $"HTTP/1.1 200 OK\r\nContent-Length: {bodyStream.Length}\r\nConnection: close\r\n\r\n";
                            var headerArray = Encoding.UTF8.GetBytes(header);
                            await response.WriteAsync(headerArray,
                                                      0, headerArray.Length);
                            await bodyStream.CopyToAsync(response);
                            await response.FlushAsync();
                        }
                    }
                }*/

                //POST to the PolisServer
                if (query.Contains("thingTag"))
                    await postData.PostAsync(GetQueryObject(query));
            }

            //Return the query string
            private static string GetQueryString(StringBuilder request)
            {
                var requestLines = request.ToString().Split(' ');

                var url = requestLines.Length > 1
                                  ? requestLines[1] : string.Empty;

                var uri = new Uri("http://localhost" + url);
                var query = uri.Query;
                return query;
            }

            //Return the query object
            private static object GetQueryObject(string query)
            {
                try
                {
                    if (query.Contains("%"))
                        query = query.Split('%')[0];

                    string parameterString = query.Split('?')[1];
                    string[] parameters = parameterString.Split('&');
                    
                    return (new
                    {
                        thingTag = parameters[0].Split('=')[1],
                        metricTag = parameters[1].Split('=')[1],
                        value = parameters[2].Split('=')[1]
                    });

                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }

                return (new
                {
                    data = "null"
                });
            }
        }
        #endregion
    }
}

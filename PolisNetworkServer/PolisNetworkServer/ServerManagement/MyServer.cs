using System;
using System.Text;
using System.Diagnostics;
using System.Runtime.InteropServices.WindowsRuntime;

using Windows.Storage.Streams;
using Windows.Networking.Sockets;

namespace PolisNetworkServer
{
	class MyServer
	{
		HTTPHandler postData = new HTTPHandler(CommonElements.urlPublishLog);
		private const uint BufferSize = 8192;

		public async void Start()
		{
			StreamSocketListener listener = new StreamSocketListener();

			await listener.BindServiceNameAsync("80");
			listener.ConnectionReceived += Listener_ConnectionReceived;
		}

		private async void Listener_ConnectionReceived(StreamSocketListener sender, StreamSocketListenerConnectionReceivedEventArgs args)
		{
			StringBuilder Request = new StringBuilder();

			using (var input = args.Socket.InputStream)
			{
				var data = new byte[BufferSize];
				IBuffer buffer = data.AsBuffer();
				var dataRead = BufferSize;

				while (dataRead == BufferSize)
				{
					await input.ReadAsync(buffer, BufferSize, InputStreamOptions.Partial);
					Request.Append(Encoding.UTF8.GetString(data, 0, data.Length));
					dataRead = buffer.Length;
				}
			}

			string Query = GetQueryString(Request);

			if (Query.Contains("thingTag"))
				await postData.PostAsync(GetQueryObject(Query));
		}

		private static string GetQueryString(StringBuilder Request)
		{
			string[] RequestStrings = Request.ToString().Split(' ');

			string Url = RequestStrings.Length > 1
							  ? RequestStrings[1] : string.Empty;

			Uri UriQuery = new Uri("http://localhost" + Url);

			return UriQuery.Query;
		}

		private static object GetQueryObject(string Query)
		{
			try
			{
				if (Query.Contains("%"))
					Query = Query.Split('%')[0];

				string parameterString = Query.Split('?')[1];
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
}

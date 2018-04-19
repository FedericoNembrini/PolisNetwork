using Flurl.Http;
using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace PolisNetworkHub_HTTPServer
{
    class HTTPRequestHandler
    {
        private string url;

        public HTTPRequestHandler(string url)
        {
            this.url = url;
        }

        public async Task PostAsync(object dati)
        {
            try
            {
                var response = await url
                              .PostUrlEncodedAsync(dati)
                              .ReceiveString();
                Debug.WriteLine(response);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }
    }
}

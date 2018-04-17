using System.Threading.Tasks;

using Flurl.Http;

namespace PolisNetworkHub_HTTPServer
{
    class HTTPRequestHandler
    {
        private string url;

        public HTTPRequestHandler(string url)
        {
            this.url = url;
        }

        public async Task<string> Post(object dati)
        {
            var risposta = await url
                .PostUrlEncodedAsync(dati)
                .ReceiveString();

            return risposta;
        }
    }
}

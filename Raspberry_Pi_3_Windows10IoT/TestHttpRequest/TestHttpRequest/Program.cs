using System;
using Flurl.Http;
using System.Threading.Tasks;

namespace TestHttpRequest
{
    class Program
    {
        static void Main(string[] args)
        {
            new Program().Post().Wait();
        }

        private async Task Post()
        {
            string url = "http://polis.inno-school.org/polis/php/api/publishMetric.php";
            var risposta = "aa";
            risposta = await url
                .PostUrlEncodedAsync(new { thingTag = "cccccccccccc", metricTag = "daaaaaaaaaaa", value = "20" })
                .ReceiveString();
            Console.WriteLine(risposta);
        }
    }
}

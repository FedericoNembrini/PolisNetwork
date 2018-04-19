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
            
            /*string url = "http://polis.inno-school.org/polis/php/api/login.php";
            var risposta = "aa";
            risposta = await url
                .PostUrlEncodedAsync(new { data = "{\"user\":\"admin\",\"pass\":\"password\"}" })
                .ReceiveString();
            Console.WriteLine(risposta);

            url = "http://polis.inno-school.org/polis/php/api/getUserThingList.php?data=a";
            risposta = "aa";
            risposta = await url
                .GetStringAsync();
            Console.WriteLine(risposta);*/
            string url = "http://polis.inno-school.org/polis/php/api/publishMetric.php";
            await url
                    .PostUrlEncodedAsync(new { thingTag = "cccccccccccc", metricTag = "daaaaaaaaaaa", value = "20" })
                    .ReceiveString();
        }
    }
}

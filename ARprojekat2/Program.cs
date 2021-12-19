using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace ARprojekat2
{
    class Program
    {
        //obicni quicksort, pivot je element koji se nalazi skroz desno
        private static void Quicksort(int[] elements, int left, int right)
        {
            if (left < right)
            {
                int pivot = Partition(elements, left, right);
                Quicksort(elements, left, pivot - 1);
                Quicksort(elements, pivot + 1, right);
            }
        }

        public static int Partition(int[] elements, int left, int right)
        {
            int pivot = elements[right];

              int i = left;
              int j = right - 1;
              while (i < j)
              {
                  //pronadji prvi element koji je veci ili jednak pivotu
                  while (elements[i] < pivot)
                  {
                      i++;
                  }
                  // pronadji prvi element zdesna koji je manji od pivota
                  while (j > left && elements[j] >= pivot)
                  {
                      j--;
                  }
                  // ako je veci element lijevo od manjeg elementa zamijeni ih
                  if (i < j)
                  {
                        int temp = elements[i];
                        elements[i] = elements[j];
                        elements[j] = temp;
                        i++;
                      j--;
                  }
              }
              // i == j znaci da jos uvijek nismo provjerili ovaj element
              //pomjeri i tako da pokazuje na pocetak desnog niza
              if (i == j && elements[i] < pivot)
              {
                  i++;
              }

              // pomjeri pivot na finalnu poziciju
              if (elements[i] != pivot)
              {
                    int temp = elements[i];
                    elements[i] = elements[right];
                    elements[right] = temp;
              }
              return i;
        }

        //quicksort sa paralelizacijom na visejezgarnom procesoru
        private static void ParallelQuicksort(int[] elements, int left, int right)
        {
            //bio bi previse velik broj paralelnih tokova
            if (right - left >= 2000)
            {
                int pivot = Partition(elements, left, right);
                Parallel.Invoke(
                    () => ParallelQuicksort(elements, left, pivot - 1),
                    () => ParallelQuicksort(elements, pivot + 1, right));
            }
            else
            {
                Quicksort(elements, left, right);

            }
           
        }

        private static void DualPivotQuicksort(int[] elements, int left, int right)
        {
            if (left < right)
            {
                int[] pivots = DualPivotPartition(elements, left, right);
                int p0 = pivots[0];
                int p1 = pivots[1];
                DualPivotQuicksort(elements, left, p0 - 1);
                DualPivotQuicksort(elements, p0 + 1, p1 - 1);
                DualPivotQuicksort(elements, p1 + 1, right);
            }
        }

        private static int[] DualPivotPartition(int[] elements, int left, int right)
        {
            if (elements[left] > elements[right])
            {
                int tmp = elements[left];
                elements[left] = elements[right];
                elements[right] = tmp;

            }
            int j = left + 1;
            int g = right - 1; int k = left + 1;
            int p = elements[left]; int q = elements[right];

            while (k <= g)
            {
                if(elements[k] < p) {
                    int tmp = elements[k];
                    elements[k] = elements[j];
                    elements[j] = tmp;
                    j++;

                }
                else if (elements[k] >= q)
                {
                    while (elements[g] > q && k < g) g--;
                    int tmp = elements[k];
                    elements[k] = elements[g];
                    elements[g] = tmp;
                    g--;
                    if (elements[k] < p)
                    {
                         tmp = elements[k];
                        elements[k] = elements[j];
                        elements[j] = tmp;
                        j++; 
                    }

                }
                k++;
            }
            j--;
            g++;

            int temp = elements[left];
            elements[left] = elements[j];
            elements[j] = temp;

            temp = elements[right];
            elements[right] = elements[g];
            elements[g] = temp;
    
            return new int[] { j, g };
        }

        private static void DualPivotParallelQuicksort(int[] elements, int left, int right)
        {
            if (right - left >= 2000)
            {
                int[] pivots = DualPivotPartition(elements, left, right);
                int p0 = pivots[0];
                int p1 = pivots[1];
                Parallel.Invoke(
                   () => DualPivotParallelQuicksort(elements, left, p0 - 1),
                   () => DualPivotParallelQuicksort(elements, p0 + 1, p1 - 1),
                   () => DualPivotParallelQuicksort(elements, p1 + 1, right));
            }
            else
            {
                DualPivotQuicksort(elements, left, right);
            }
        }


        private static int[] GenerateArray(int numOfElements)
        {
            Random randNum = new Random();
            int[] elements = Enumerable
                .Repeat(0, numOfElements)
                .Select(i => randNum.Next())
                .ToArray();
            return elements;
        }

        static void Main(string[] args)
        {
            int numOfElements =10000000;
           
            int[] elements = GenerateArray(numOfElements);

         /*   int[] elements = new int[numOfElements];
            string[] lines = System.IO.File.ReadAllLines("niz.txt");
            for(int i=0; i<lines.Length; i++)
            {
               elements[i] = Int32.Parse(lines[i]);   
            }*/

            int[] a1= new int[numOfElements];
            Array.Copy(elements, a1, elements.Length);

            int[] a2 = new int[numOfElements];
            Array.Copy(elements, a2, elements.Length);

            int[] a3 = new int[numOfElements];
            Array.Copy(elements, a3, elements.Length);


        /*    using (FileStream fs = File.Open("niz.txt", FileMode.Create))
            {
                StreamWriter sw = new StreamWriter(fs);
                elements.ToList().ForEach(e => sw.WriteLine(e));
            }*/
           

            Stopwatch sw1 = Stopwatch.StartNew();
            Quicksort(elements, 0, elements.Length - 1);
            Console.WriteLine(sw1.ElapsedMilliseconds);
            sw1.Stop();

            Stopwatch sw2 = Stopwatch.StartNew();
            ParallelQuicksort(a1, 0, a1.Length - 1);
            Console.WriteLine(sw2.ElapsedMilliseconds);
            sw2.Stop();

            Stopwatch sw3 = Stopwatch.StartNew();
            DualPivotQuicksort(a2, 0, a2.Length - 1);
            Console.WriteLine(sw3.ElapsedMilliseconds);
            sw3.Stop();

            Stopwatch sw4 = Stopwatch.StartNew();
            DualPivotParallelQuicksort(a3, 0, a3.Length - 1);
            Console.WriteLine(sw4.ElapsedMilliseconds);
            sw4.Stop();
        }
    }
}

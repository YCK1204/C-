namespace SpinLock
{
    class SpinLock
    {
        static volatile int _lock = 0;
        static int expectedValue = 0;
        static int desireValue = 1;

        public static void TurnOn()
        {
            while (Interlocked.CompareExchange(ref _lock, desireValue, expectedValue) == desireValue);
            _lock = desireValue;
        }

        public static void TurnOff()
        {
            _lock = 0;
        }
    }

    class Program
    {
        static int num = 0;

        static void Increase()
        {
            for (int i = 0; i < 1000000; i++)
            {
            Console.WriteLine("increase");
                SpinLock.TurnOn();
                num++;
                SpinLock.TurnOff();
            }
        }

        static void Decrease()
        {

            for(int i = 0; i < 1000000; i++)
            {
            Console.WriteLine("decrease");
                SpinLock.TurnOn();
                num--;
                SpinLock.TurnOff();
            }
        }

        static void Main(string[] args)
        {
            Task t1 = new Task(Increase);
            Task t2 = new Task(Decrease);
            t1.Start();
            t2.Start();

            Task.WaitAll(t1, t2);
            Console.WriteLine(num);
        }
    }
}

namespace ReadWriteLock {
    class Program {
        static int num = 0;
        const int empty = 0x00000000;
        const int writeFlag = 0x7FFF0000;
        const int readFlag = 0x0000FFFF;
        static int flag = empty;
        static int writeCount = 0;
        static int readCount = 0;

        const int MAX_SPIN_COUNT = 5000;

        static void WriteLock() {
            if ((flag & writeFlag) == Thread.CurrentThread.ManagedThreadId) {
                Interlocked.Increment(ref writeCount);
                return ;
            }

            while (true) {
                int currentThreadId = Thread.CurrentThread.ManagedThreadId << 16 & writeFlag;
                for (int i = 0; i < MAX_SPIN_COUNT; i++) {
                    if (Interlocked.CompareExchange(ref flag, currentThreadId, empty) == empty) {
                        writeCount = 1;
                        return;
                    }
                }
                Thread.Yield();
            }
        }

        static void WriteUnLock() {
            if (writeCount > 0) 
            {
                if (--writeCount == 0)
                    Interlocked.Exchange(ref flag, empty);
            }
        }

        static void ReadLock() {
            if ((flag & writeFlag) != empty) {
                readCount++;
                return ;
            }

            while (true) {
                for (int i = 0; i < MAX_SPIN_COUNT && (flag & writeFlag) == 0; i++) {
                        int expectedValue = flag & readFlag;
                        if (Interlocked.CompareExchange(ref flag, expectedValue + 1, expectedValue) == expectedValue) {
                            readCount = 1;
                            return ;
                        }
                }
                Thread.Yield();
            }
        }

        static void ReadUnLock() {
            if (readCount > 0)
            {
                if (--readCount == 0)
                    Interlocked.Exchange(ref flag, flag & writeFlag);
            }
        }
        
        static void Increase() {
            for (int i = 0; i < 10000000; i++) {
                WriteLock();
                num++;
                WriteUnLock();
            }
        }
        
        static void Decrease() {
            for (int i = 0; i < 10000000; i++) {
                WriteLock();
                num--;
                WriteUnLock();
            }
        }
        
        static void Main(string[] argv) {
            Task t1 = new Task(Increase);
            Task t2 = new Task(Decrease);

            t1.Start();
            t2.Start();

            Task.WaitAll(t1, t2);
            Console.WriteLine(num);
        }
    }
}

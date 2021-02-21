#pragma once
#include "UThreadPool.h"

namespace UEngine
{
    namespace Utility
    {
        class UThreadSync : UEngine::Utility::UThreadPool
        {
        private:
            std::mutex mutex;
            unsigned count{ 0 };

        public:
            template <typename Lambda>
            void AddSyncTask(Lambda function)
            {
                std::unique_lock<std::mutex> lock(mutex);
                count++;
                lock.unlock();

                this->AddTask(function());

                lock.lock();
                count--;
                lock.unlock();
            }

            void Join()
            {
                // join threads
                while (true)
                {
                    mutex.lock();
                    unsigned count = this->count;
                    mutex.unlock();

                    if (!count)
                        break;
                }
            }
        };
    }
}

#include "stdafx.h"
#include "windows.h"
#include "vector"
#include "string"
#include "iostream"

class ThreadPool {
public:
	ThreadPool(int count) {
		if (count < 0) {
			logWarn("Thread pool size < 0");
			threadCount = DEFAULT_THREAD_COUNT;
		}
		else {
			threadCount = count;
		}

		HANDLE tempThread;
		for (int i = 0; i < threadCount; i++) {
			tempThread = CreateThread(NULL, 0, threadWork, static_cast<LPVOID>(this), 0, NULL);
			threads.push_back(tempThread);
		}

		logInfo(threadCount + " threads started");
	}

private:
	const int DEFAULT_THREAD_COUNT = 1;

	int threadCount;
	std::vector<HANDLE> threads;

	static DWORD WINAPI threadWork(LPVOID lpParam) {

	}
	
	static void logInfo(std::string message) {
		std::cout << "[INFO]: " << message;
	}

	static void logWarn(std::string message) {
		std::cout << "[WARN]: " << message;
	}

	static void logError(std::string message) {
		std::cout << "[ERROR]: " << message;
	}
};

int main()
{
	int threadPoolSize;

	std::cout << "Please, enter thread pool size: ";
	std::cin >> threadPoolSize;

	ThreadPool threadPool = ThreadPool(threadPoolSize);

	std::cout << "Please choose task to execute(1 - task1, 2 - task2, 3 - task3)";
}


#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include <atomic>

struct API {
  static int counter;
  std::string readMessage(int i){
    std::stringstream ss;
    ss << "msg: " << i;
    auto msg = ss.str();
    return msg;
  }
  void sendMessage(const std::string& str){    
    std::cout << "sendMessage: " << str << std::endl;
  }
};

class TimedTaskProcessor {
private:
  std::atomic<bool> stop_;


public:
  TimedTaskProcessor():stop_(false){}
  void stop(){ stop_ = true; }
  void executeTask(const std::chrono::milliseconds &sleepTime) {
    API api;
    int messages_per_seconds = 50;
    // Note: want to truncate to avoid floating point.
    auto message_per_milliseconds =
        std::chrono::milliseconds(1000 / messages_per_seconds);

    while (!stop_) {

      for (int i = 0; i < messages_per_seconds; i++) {

        auto start = std::chrono::steady_clock::now();
        auto deadline = start + message_per_milliseconds;

        auto msg = api.readMessage(i);
        api.sendMessage(msg);
        if (std::chrono::steady_clock::now() < deadline) {
          std::this_thread::yield();
          std::this_thread::sleep_until(deadline);
        }
      }
      std::this_thread::sleep_for(sleepTime);
    }
  }
};

int main() {
  TimedTaskProcessor p;
  auto processThread = std::thread([&p]{p.executeTask(std::chrono::milliseconds(100));});
  std::this_thread::sleep_for(std::chrono::seconds(1));
  p.stop();
  if(processThread.joinable()){
    processThread.join();
  }
  return 0;
}

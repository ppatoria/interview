#include <sstream>
#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>
#include <string>

struct API {

  std::string readMessage(int i){
    std::stringstream ss;
    ss << "msg: " << i;
    return ss.str();
  }

  void sendMessage(const std::string& msg){
    std::cout << "Send: " << msg << std::endl;
  }
};

std::atomic<bool> stop;
void timedTaskProcess(std::chrono::milliseconds sleepTime, int numberOfMessagesPerSecond){

  API api;
  using clock = std::chrono::steady_clock;
  auto timePerMessage = std::chrono::duration<double, std::milli>(1000.0/numberOfMessagesPerSecond);
  while(!stop){
    for(int i=0;i<numberOfMessagesPerSecond; ++i){
      auto start_time = clock::now();
      auto deadline = start_time + timePerMessage;

      api.sendMessage(api.readMessage(i));

      if(clock::now() < deadline){
        std::this_thread::sleep_until(deadline);
      }
    }
    std::this_thread::sleep_for(sleepTime);
  }
}

int main(){
  stop = false;
  auto th = std::thread([]{
    timedTaskProcess(std::chrono::milliseconds(100), 50);
  });

  std::this_thread::sleep_for(std::chrono::seconds(1));
  stop = true;
  if(th.joinable())
    th.join();
  
  return 0;
}

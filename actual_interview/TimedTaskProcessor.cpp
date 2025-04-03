#include <chrono>
#include <atomic>
#include <thread>
#include <string>
#include <iostream>
#include <functional>
#include <sstream>

struct API {
  std::string read(int i){
    std::stringstream ss;
    ss << "msg: " << i;
    return ss.str();    
  }

  void send(const std::string& msg){
    std::cout << msg << " sent." << std::endl;
  }
};



class TimedTaskProcessor {
 private:
  std::atomic<bool> stop_;
  std::thread workerThread_;

 public :
  TimedTaskProcessor(): stop_(false){}

  void startTask(std::function<void(int)>&& task, int tasksPerSecond, std::chrono::milliseconds sleepTime ){
    workerThread_ = std::thread([this, task = std::move(task), tasksPerSecond, sleepTime]{
      auto timePerTask = std::chrono::duration<double, std::milli>(1000.0/tasksPerSecond);
      while(!stop_){
        for(int i=0;i<tasksPerSecond;i++){
          auto start_time = std::chrono::steady_clock::now();
          auto deadline = start_time + timePerTask;
          task(i);
          if(std::chrono::steady_clock::now() < deadline){
            std::this_thread::yield();
            std::this_thread::sleep_until(deadline);
          }
        }
        std::this_thread::sleep_for(sleepTime);
      }
    });
  }

  void stop(){
    stop_ = true;
  }

  bool isRunning(){
    return !stop_;
  }

  ~TimedTaskProcessor(){
    stop();
    std::this_thread::yield();
    if(workerThread_.joinable())
      workerThread_.join();
  }
};

int main(){
  TimedTaskProcessor processor;
  API api;
  processor.startTask(
      [&api] (int i){
        auto msg = api.read(i);
        api.send(msg);
      },
      50,
      std::chrono::milliseconds(100));
  std::this_thread::sleep_for(std::chrono::seconds(2));

}

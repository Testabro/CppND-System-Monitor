#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization(); 

 private:
    long _prev_active_jiffs;
    long _prev_idle_jiffs;
};

#endif
#ifndef UPLOADER_DATA_HPP
#define UPLOADER_DATA_HPP

class UploaderData {
public:
  enum UploaderCmdType { CONNECT_CMD = 1, SEND_CMD = 2, CANCEL_CMD = 3, DISCONNECT_CMD = 4 };
  
  typedef void (*ProgressFunc)(int, int);
  
  UploaderData (UploaderCmdType cmd, std::string message = "", int robotId = 0, ProgressFunc func = NULL) 
    : mCmd(cmd), mRobotId(robotId), mMessage(message), mUpdateProgressFunc(func) {};
    
  UploaderCmdType cmd() const             { return mCmd; }
  std::string message() const             { return mMessage; }
  int robotId() const                     { return mRobotId; }
  ProgressFunc updateProgressFunc() const { return mUpdateProgressFunc; }
    
private:
  UploaderCmdType mCmd;
  int mRobotId;
  std::string mMessage;
  ProgressFunc mUpdateProgressFunc;
};
  
#endif // UPLOADER_DATA_HPP

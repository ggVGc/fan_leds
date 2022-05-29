
void setup_server();
void server_loop_step();

enum MessageType { 
  None,
  OneShot,
  Loop
};

MessageType get_message_type();
const String& get_message();

void ap_mode(){


 WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
   // Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
          h = dht.readHumidity();
          t = dht.readTemperature(); 
          Serial.print(t); 
          Serial.println("C  ");
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE html><html><head><meta http-equiv='refresh' content='3'><title>MRC 2019</title></head><body>");
          client.println("<H1>");
          client.println(t);
          client.println(" &deg;C</H1></body></HTML>");
          int n =1;
          //Serial.println("scan done");
          
          if (header.indexOf("/action_page.php?") >= 0) {

          }
          
          
          else if (n == 0) {
            client.println("network not found");
            break;
          } else {
    
          }      
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    delay(100);
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }


  
}

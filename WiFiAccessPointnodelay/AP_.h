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

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          int n = WiFi.scanNetworks();
          //Serial.println("scan done");
          
          if (header.indexOf("/action_page.php?") >= 0) {

            String result = url_encode(header);
            String mSsid  = result.substring(result.indexOf("ssid=")+5,result.indexOf("&"));
            String mPass  = result.substring(result.indexOf("&")+1,result.indexOf("\n",result.indexOf("&")+1));
            //Serial.println("++++++++++++++++++++++++++++++++++++++");
            //Serial.println(mSsid);
            //Serial.println(mPass);
            eeprom_clear(0,110);
            write_eeprom(0,mSsid);
            write_eeprom(32,mPass);
            write_eeprom(100,"client");
            //Serial.println("++++++++++++++++++++++++++++++++++++++");
            client.println("done........");
            delay(3000);
            ESP.restart();
            break;
          }
          
          
          else if (n == 0) {
            client.println("network not found");
            break;
          } else {
            //Serial.print(n);
            //Serial.println(" networks found");
            
            client.println("<html><style>input[type=text], select {width: 100%;padding: 12px 20px; margin: 8px 0;  display: inline-block;  border: 1px solid #ccc;  border-radius: 4px; box-sizing: border-box;}");
            client.println("input[type=submit] {  width: 100%;  background-color: #4CAF50;  color: white; padding: 14px 20px;  margin: 8px 0;  border: none;  border-radius: 4px;  cursor: pointer;}");
            client.println("input[type=submit]:hover {  background-color: #45a049;} div {  border-radius: 5px;  background-color: #f2f2f2;  padding: 20px;}");
            client.println("</style><body><h3>Setting Device</h3><div>  <form action='/action_page.php'> <label for='country'>SSID</label>    <select id='ssid' name='ssid'> ");
            for (int i = 0; i < n; ++i) {
              // Print SSID and RSSI for each network found
              client.print("<option value='");
              client.print(WiFi.SSID(i));
              client.print("'>");
              client.print(WiFi.SSID(i));
              client.print(" </option>");
              
            
            }
            client.println("");
            client.print("</select><label for='lname'>Last Name</label> <input type='text' id='lname' name='password' placeholder='password'><input type='submit' value='Submit'></form></div></body></html>");
          
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
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }


  
}

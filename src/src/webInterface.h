
#include <Arduino.h>

//
// =======================================================================================================
// WEB INTERFACE
// =======================================================================================================
//

// Emit one "Sound Volume" slider (DieselCore stepper card via the page's JS) and
// apply the value if this request set it. `var` is the vehicle's volume variable.
void volSlider(WiFiClient &client, String &header, const char *param, const char *label, volatile int &var)
{
  String vs = String((int)var, DEC);
  client.println(String("<p>") + label + ": <span id=\"textSlider" + param + "Value\">" + vs + "</span><br>");
  client.println(String("<input type=\"range\" min=\"0\" max=\"200\" step=\"5\" class=\"slider\" id=\"Slider") + param + "Input\" onchange=\"vol('" + param + "',this.value)\" value=\"" + vs + "\" /></p>");
  if (header.indexOf(String("GET /?") + param + "=") >= 0)
  {
    int p1 = header.indexOf('=');
    int p2 = header.indexOf('&');
    if (p1 >= 0 && p2 > p1)
      var = header.substring(p1 + 1, p2).toInt();
  }
}

void webInterface()
{

  static unsigned long currentTime = millis(); // Current time
  static unsigned long previousTime = 0;       // Previous time
  const long timeoutTime = 2000;               // Define timeout time in milliseconds (example: 2000ms = 2s)

  static bool Mode = false; // TODO

  if (true)
  { // Wifi on
    // if (WIFI_ON == 1) {     //Wifi on
    WiFiClient client = server.available(); // Listen for incoming clients

    if (client)
    { // If a new client connects,
      currentTime = millis();
      previousTime = currentTime;
      Serial.println("New Client."); // print a message out in the serial port
      String currentLine = "";       // make a String to hold incoming data from the client
      while (client.connected() && currentTime - previousTime <= timeoutTime)
      { // loop while the client's connected
        currentTime = millis();
        if (client.available())
        {                         // if there's bytes to read from the client,
          char c = client.read(); // read a byte, then
          Serial.write(c);        // print it out the serial monitor
          header += c;
          if (c == '\n')
          { // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0)
            {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<title>TheDIYGuy999 Sound & Light Controller</title>");

#if defined USE_CSS
              // CSS styles for buttons

#if defined MODERN_CSS // The modern CSS with scaling for better adaption between different devices
              // ==== TheDIYGuy999 flasher skin (neon green + hot pink, DieselCore-style) ====
              client.println("<style>");
              client.println("html{font-family:'Trebuchet MS',Arial,sans-serif;margin:0;text-align:center;color:#f3ffe6;background:#0a1206;background-image:radial-gradient(900px 500px at 15% -10%,rgba(57,255,20,.22),transparent 60%),radial-gradient(800px 600px at 100% 0%,rgba(255,46,151,.18),transparent 55%),linear-gradient(160deg,#0a1206,#112011);background-attachment:fixed;min-height:100%;}");
              client.println("body{margin:0;padding:0 12px 40px;}");
              client.println("h1{font-family:Impact,'Arial Black',sans-serif;text-transform:uppercase;font-style:italic;letter-spacing:1px;color:#ff2e97;text-shadow:0 0 12px rgba(255,46,151,.7);font-size:clamp(1.6rem,5vw,2.4rem);margin:16px 0 2px;}");
              client.println("h2{font-family:Impact,'Arial Black',sans-serif;text-transform:uppercase;letter-spacing:.5px;color:#39ff14;font-size:clamp(1.15rem,3.5vw,1.6rem);margin:4px 0;}");
              client.println("p{font-size:clamp(.95rem,2.8vw,1.15rem);color:#d7ffc9;}");
              client.println("label{font-size:clamp(.95rem,2.8vw,1.15rem);color:#d7ffc9;}");
              client.println("a{color:#39ff14;}");
              client.println("hr{border:none;border-top:2px solid #3f9f24;margin:16px 0;}");
              client.println(".multiColumn{display:inline-block;width:49%;text-align:left;vertical-align:top;}");
              client.println("input[type=\"checkbox\"]{-webkit-appearance:none;appearance:none;width:56px;height:30px;border-radius:999px;background:rgba(8,16,6,.85);border:2px solid #3f9f24;position:relative;cursor:pointer;vertical-align:middle;margin:4px 8px;flex:0 0 auto;transition:background .15s,border-color .15s;}");
              client.println("input[type=\"checkbox\"]::after{content:'';position:absolute;top:2px;left:2px;width:22px;height:22px;border-radius:50%;background:#7A7A7A;transition:left .15s,background .15s;box-shadow:0 1px 3px rgba(0,0,0,.5);}");
              client.println("input[type=\"checkbox\"]:checked{background:linear-gradient(135deg,#39ff14,#8fff5a);border-color:#39ff14;box-shadow:0 0 10px rgba(57,255,20,.5);}");
              client.println("input[type=\"checkbox\"]:checked::after{left:28px;background:#0a1206;}");
              client.println(".slider{-webkit-appearance:none;appearance:none;width:95%;height:22px;background:rgba(8,16,6,.85);outline:none;border:2px solid #3f9f24;margin:6px auto;border-radius:999px;pointer-events:none;}");
              client.println(".slider::-webkit-slider-thumb{-webkit-appearance:none;cursor:pointer;width:34px;height:34px;background:radial-gradient(circle at 35% 30%,#eaffe0,#39ff14);border:2px solid #faff00;border-radius:50%;box-shadow:0 0 10px rgba(57,255,20,.8);}");
              client.println(".sliderServo1::-webkit-slider-thumb{background:radial-gradient(circle at 35% 30%,#ffd0e4,#ff2e97);border-color:#fff;}");
              client.println(".sliderServo2::-webkit-slider-thumb{background:radial-gradient(circle at 35% 30%,#cfeaff,#00b3ff);border-color:#fff;}");
              client.println(".sliderServo3::-webkit-slider-thumb{background:radial-gradient(circle at 35% 30%,#ffe6cf,#ff9b2e);border-color:#fff;}");
              client.println(".sliderLed::-webkit-slider-thumb{background:radial-gradient(circle at 35% 30%,#ffffcf,#faff00);border-color:#fff;}");
              client.println(".collapsible{background:linear-gradient(180deg,#12260c,#0a1a06);color:#39ff14;cursor:pointer;padding:14px;width:100%;border:2px solid #3f9f24;border-radius:14px;text-align:center;outline:none;font-family:Impact,'Arial Black',sans-serif;text-transform:uppercase;letter-spacing:1px;font-size:clamp(1rem,3vw,1.3rem);margin-top:10px;}");
              client.println(".active,.collapsible:hover{border-color:#39ff14;color:#eaffe0;box-shadow:0 0 16px rgba(57,255,20,.4);}");
              client.println(".content{display:none;padding:8px;border:2px solid #3f9f24;border-top:none;border-radius:0 0 14px 14px;background:rgba(18,38,12,.55);margin-bottom:8px;}");
              client.println(".textbox{cursor:pointer;border:2px solid #3f9f24;background:rgba(8,16,6,.7);color:#f3ffe6;font-size:clamp(1rem,3vw,1.35rem);padding:8px 12px;text-align:center;border-radius:10px;}");
              client.println(".button{cursor:pointer;border:2px solid #3f9f24;padding:12px 18px;margin:8px 0;font-family:'Trebuchet MS',Arial,sans-serif;font-weight:800;font-size:clamp(1rem,3vw,1.3rem);width:95%;border-radius:999px;color:#f3ffe6;background:rgba(28,56,18,.6);}");
              client.println(".buttonGreen{background:linear-gradient(135deg,#39ff14,#8fff5a);color:#0a1206;border:0;box-shadow:0 0 14px rgba(57,255,20,.6);}");
              client.println(".buttonRed{background:linear-gradient(135deg,#ff2e97,#ff5b6b);color:#fff;border:0;box-shadow:0 0 14px rgba(255,46,151,.6);}");
              client.println(".buttonGrey{background:rgba(28,56,18,.6);color:#d7ffc9;}");
              // DieselCore-style stepper slider cards (built by JS on load)
              client.println(".slidercard{background:rgba(18,38,12,.55);border:2px solid #3f9f24;border-radius:16px;padding:12px 14px;margin:10px auto;max-width:660px;box-shadow:0 0 14px rgba(57,255,20,.12);text-align:left;}");
              client.println(".slwrap{display:flex;align-items:center;gap:10px;margin-top:10px;}");
              client.println(".slwrap .slider{flex:1;margin:0;width:auto;}");
              client.println(".stp{flex:0 0 auto;width:50px;height:46px;border-radius:12px;border:2px solid #3f9f24;background:linear-gradient(180deg,#12260c,#0a1a06);color:#39ff14;font-size:22px;font-weight:900;line-height:1;cursor:pointer;}");
              client.println(".stp:active{transform:translateY(1px);background:#39ff14;color:#0a1206;}");
              client.println("span[id^=\"textSlider\"],span[id^=\"textslider\"]{display:inline-block;min-width:52px;background:#faff00;color:#0a1206;font-weight:800;border-radius:999px;padding:2px 12px;margin-left:6px;box-shadow:0 0 10px rgba(250,255,0,.5);}");
              // logo bar (matches the flasher header) + retro grid overlay
              client.println(".logobar{background:#0a1206;padding:12px 0 8px;margin:0 -12px 8px;border-bottom:2px solid #39ff14;box-shadow:0 4px 22px rgba(57,255,20,.3);}");
              client.println(".logobar img{width:min(280px,86vw);height:auto;display:block;margin:0 auto;}");
              client.println("body::before{content:'';position:fixed;inset:0;z-index:-1;pointer-events:none;background-image:linear-gradient(rgba(57,255,20,.08) 1px,transparent 1px),linear-gradient(90deg,rgba(250,255,0,.07) 1px,transparent 1px);background-size:44px 44px;-webkit-mask-image:linear-gradient(transparent,#000 35%);mask-image:linear-gradient(transparent,#000 35%);}");
              client.println("</style></head>");

#else // Old CSS with green background
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: rgb(60, 161, 120);}");
              client.println(".button { border: yes; color: white; padding: 10px 40px; width: 95%;");
              client.println("text-decoration: none; font-size: 16px; margin: 2px; cursor: pointer;}");
              client.println(".slider { -webkit-appearance: none; width: 95%; height: 25px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s; }");
              client.println(".buttonGreen {background-color: #4CAF50;}");
              client.println(".buttonRed {background-color: #ff0000;}");
              client.println(".buttonGrey {background-color: #7A7A7A;}");
              client.println(".textbox {font-size: 16px; text-align: center;}");
              client.println("</style></head>");
#endif
#endif

              client.println("</head>");

              client.println("<body onload=\"readDefaults()\">");

              // vol(): send a Sound Volume change.  Slider enhancer: DieselCore stepper cards + thumb-only drag.
              client.println("<script>");
              client.println("function vol(p,pos){var e=document.getElementById('textSlider'+p+'Value');if(e)e.innerHTML=pos;var x=new XMLHttpRequest();x.open('GET','/?'+p+'='+pos+'&',true);x.send();}");
              client.println("window.addEventListener('load',function(){var s=document.querySelectorAll('input[type=range].slider');for(var i=0;i<s.length;i++){(function(sl){var st=parseFloat(sl.getAttribute('step'))||1;var mn=parseFloat(sl.getAttribute('min'));var mx=parseFloat(sl.getAttribute('max'));var w=document.createElement('div');w.className='slwrap';var a=document.createElement('button');a.type='button';a.className='stp';a.innerHTML='&#9666;';var b=document.createElement('button');b.type='button';b.className='stp';b.innerHTML='&#9656;';sl.parentNode.insertBefore(w,sl);w.appendChild(a);w.appendChild(sl);w.appendChild(b);function n(d){var v=parseFloat(sl.value)+d*st;if(!isNaN(mn))v=Math.max(mn,v);if(!isNaN(mx))v=Math.min(mx,v);sl.value=v;sl.dispatchEvent(new Event('change'));}a.onclick=function(){n(-1);};b.onclick=function(){n(1);};var c=sl.closest('p');if(c)c.classList.add('slidercard');})(s[i]);}});");
              // DieselCore drag: slider is pointer-events:none; drag only starts on the thumb, else the page scrolls
              client.println("(function(){var drag=null,R=30;function thumbX(inp){var r=inp.getBoundingClientRect();var mn=+inp.min||0,mx=+inp.max||100,v=+inp.value||0;var f=mx>mn?(v-mn)/(mx-mn):0;return{r:r,x:r.left+17+f*(r.width-34)};}function apply(inp,cx){var mn=+inp.min||0,mx=+inp.max||100,st=+inp.step||1;var r=inp.getBoundingClientRect();var f=Math.max(0,Math.min(1,(cx-r.left-17)/(r.width-34)));var v=mn+f*(mx-mn);v=Math.round(v/st)*st;inp.value=v;var pill=document.getElementById('text'+inp.id.replace('Input','Value'));if(pill)pill.innerHTML=inp.value;}document.addEventListener('pointerdown',function(e){var h=e.target.closest?e.target.closest('.slwrap'):null;var inp=h?h.querySelector('input[type=range]'):null;if(!inp)return;var t=thumbX(inp);if(e.clientY<t.r.top-16||e.clientY>t.r.bottom+16)return;if(Math.abs(e.clientX-t.x)>R)return;drag=inp;e.preventDefault();},true);document.addEventListener('pointermove',function(e){if(drag){apply(drag,e.clientX);e.preventDefault();}},{passive:false});function end(){if(drag){drag.dispatchEvent(new Event('change'));drag=null;}}document.addEventListener('pointerup',end,true);document.addEventListener('pointercancel',function(){drag=null;},true);document.addEventListener('touchmove',function(e){if(drag)e.preventDefault();},{passive:false,capture:true});})();");
              client.println("</script>");

              // ==== flasher logo header (embedded JPEG) ====
              client.print("<div class=\"logobar\"><img alt=\"TheDIYGuy999\" src=\"data:image/jpeg;base64,");
              client.print("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");
              client.println("\"></div>");
              // client.printf("<p>Vehicle: %s\n", ssid); // TODO, not working!
              client.printf("<p>Software version: %s\n", codeVersion);
              client.printf("<p style=\"color:red;\"><b>Don't mess around while driving!</b></p>");

              // ===== Master Volume (live web override) =====
              client.println("<hr>");
              valueString = String(masterVolume, DEC);
              client.println("<p style=\"font-family:Impact,'Arial Black',sans-serif;text-transform:uppercase;color:#39ff14;letter-spacing:1px;font-size:clamp(1.1rem,3.5vw,1.5rem);\">&#x1F50A; Master Volume: <span id=\"textSliderMVValue\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"0\" max=\"150\" step=\"5\" class=\"slider\" id=\"MasterVolInput\" onchange=\"MasterVolChange(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function MasterVolChange(pos) { ");
              client.println("document.getElementById(\"textSliderMVValue\").innerHTML = document.getElementById(\"MasterVolInput\").value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?MasterVol=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?MasterVol=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                webMasterVolume = (valueString.toInt());
                masterVolume = webMasterVolume;
                Serial.println("masterVolume = " + String(masterVolume));
              }

              // ===== Individual sound volumes (live) =====
              client.println("<hr>");
              client.println("<button type=\"button\" class=\"collapsible\">&#x1F39A; Sound Volumes</button>");
              client.println("<div class=\"content\">");
              volSlider(client, header, "volStart", "Start", startVolumePercentage);
              volSlider(client, header, "volIdle", "Idle", idleVolumePercentage);
              volSlider(client, header, "volEngIdle", "Engine (idle throttle)", engineIdleVolumePercentage);
              volSlider(client, header, "volRev", "Rev", revVolumePercentage);
              volSlider(client, header, "volEngRev", "Engine (rev throttle)", engineRevVolumePercentage);
              volSlider(client, header, "volThrottle", "Full throttle", fullThrottleVolumePercentage);
              volSlider(client, header, "volDiesel", "Diesel knock", dieselKnockVolumePercentage);
              volSlider(client, header, "volTurbo", "Turbo", turboVolumePercentage);
              volSlider(client, header, "volJake", "Jake brake", jakeBrakeVolumePercentage);
              volSlider(client, header, "volHorn", "Horn", hornVolumePercentage);
              volSlider(client, header, "volSiren", "Siren", sirenVolumePercentage);
              volSlider(client, header, "volBrake", "Air brake", brakeVolumePercentage);
              volSlider(client, header, "volReverse", "Reversing beep", reversingVolumePercentage);
              client.println("</div>");

#if defined BATTERY_PROTECTION
              client.println("<hr>"); // Horizontal line ===================================================================================================================================================
              client.printf("<p>Battery voltage: %.2f V\n", batteryVolts());
              if (numberOfCells > 1)
              {
                client.printf("<p>Number of cells: %i (%iS battery)\n", numberOfCells, numberOfCells);
              }
              else
              {
                client.printf("<p style=\"color:red;\">Battery error!\n");
              }
              client.printf("<p>Battery cutoff voltage: %.2f V\n", batteryCutoffvoltage);
#endif

              client.println("<hr>"); // WiFi settings ===================================================================================================================================================
              client.println("<button type=\"button\" class=\"collapsible\">WiFi settings</button>");
              client.println("<div class=\"content\">");

              // Set1 (ssid) ----------------------------------
              valueString = ssid;              // Read current value
              client.println("<p>SSID: <br>"); // Display current value

              client.println("<input type=\"text\" id=\"Setting1Input\" size=\"31\" maxlength=\"31\" class=\"textbox\" oninput=\"Setting1change(this.value)\" value=\"" + valueString + "\" /></p>"); // Set new value
              client.println("<script> function Setting1change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Set1=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Set1=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                Serial.println(valueString);
                ssid = valueString;
              }

              // Set2 (password) ----------------------------------
              valueString = password;                                // Read current value
              client.println("<p>Password (min. length = 8): <br>"); // Display current value

              client.println("<input type=\"text\" id=\"Setting2Input\" size=\"31\" maxlength=\"31\" class=\"textbox\" oninput=\"Setting2change(this.value)\" value=\"" + valueString + "\" /></p>"); // Set new value
              client.println("<script> function Setting2change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Set2=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Set2=") >= 0)
              {
                pos1 = header.indexOf('='); // Start pos
                pos2 = header.indexOf('&'); // End pos
                valueString = header.substring(pos1 + 1, pos2);
                password = valueString;
              }
              client.println("</div>");

              client.println("<hr>"); // Wireless trailer settings ===================================================================================================================================================
              client.println("<button type=\"button\" class=\"collapsible\">Wireless trailer settings</button>");
              client.println("<div class=\"content\">");
              // Trailer 1 ********************************************************************************************************
              if (useTrailer1 == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr1\" checked onclick=\"CheckboxTr1Change(this.checked)\"> use trailer 1: </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr1\" unchecked onclick=\"CheckboxTr1Change(this.checked)\"> use trailer 1: </input></p>");
              }
              client.println("<script> function CheckboxTr1Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?CheckboxTr1=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?CheckboxTr1=true") >= 0)
              {
                useTrailer1 = true;
                Serial.println("Trailer 1 enabled");
#if defined ENABLE_WIRELESS
                // Add the ESP-NOW link on the spot, so the trailer goes live without a reboot.
                if (!esp_now_is_peer_exist(broadcastAddress1))
                {
                  esp_now_peer_info_t p = {};
                  memcpy(p.peer_addr, broadcastAddress1, 6);
                  p.channel = 0;
                  p.encrypt = false;
                  esp_now_add_peer(&p);
                }
#endif
              }
              else if (header.indexOf("GET /?CheckboxTr1=false") >= 0)
              {
                useTrailer1 = false;
                Serial.println("Trailer 1 disabled");
#if defined ENABLE_WIRELESS
                if (esp_now_is_peer_exist(broadcastAddress1))
                  esp_now_del_peer(broadcastAddress1); // drop the link live
#endif
              }

              // MAC0 ----------------------------------
              valueString = String(broadcastAddress1[0], HEX); // Read current value
              // client.println("<p>Custom MAC: "); // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr1Mac0change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr1Mac0change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr1Mac0Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr1Mac0Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                // Serial.println(pos1);
                // Serial.println(pos2);
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress1[0] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC1 ----------------------------------
              valueString = String(broadcastAddress1[1], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr1Mac1change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr1Mac1change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr1Mac1Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr1Mac1Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress1[1] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC2 ----------------------------------
              valueString = String(broadcastAddress1[2], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr1Mac2change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr1Mac2change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr1Mac2Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr1Mac2Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress1[2] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC3 ----------------------------------
              valueString = String(broadcastAddress1[3], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr1Mac3change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr1Mac3change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr1Mac3Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr1Mac3Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress1[3] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC4 ----------------------------------
              valueString = String(broadcastAddress1[4], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr1Mac4change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr1Mac4change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr1Mac4Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr1Mac4Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress1[4] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC5 ----------------------------------
              valueString = String(broadcastAddress1[5], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr1Mac5change(this.value)\" value=\"" + valueString + "\" /></p>"); // Set new value
              client.println("<script> function Tr1Mac5change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr1Mac5Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr1Mac5Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress1[5] = strtol(valueString.c_str(), NULL, 16);
              }

              // Trailer 2 ********************************************************************************************************
              // use trailer ----------------------------------
              if (useTrailer2 == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr2\" checked onclick=\"CheckboxTr2Change(this.checked)\"> use trailer 2: </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr2\" unchecked onclick=\"CheckboxTr2Change(this.checked)\"> use trailer 2: </input></p>");
              }
              client.println("<script> function CheckboxTr2Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?CheckboxTr2=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?CheckboxTr2=true") >= 0)
              {
                useTrailer2 = true;
                Serial.println("Trailer 2 enabled");
#if defined ENABLE_WIRELESS
                if (!esp_now_is_peer_exist(broadcastAddress2))
                {
                  esp_now_peer_info_t p = {};
                  memcpy(p.peer_addr, broadcastAddress2, 6);
                  p.channel = 0;
                  p.encrypt = false;
                  esp_now_add_peer(&p);
                }
#endif
              }
              else if (header.indexOf("GET /?CheckboxTr2=false") >= 0)
              {
                useTrailer2 = false;
                Serial.println("Trailer 2 disabled");
#if defined ENABLE_WIRELESS
                if (esp_now_is_peer_exist(broadcastAddress2))
                  esp_now_del_peer(broadcastAddress2);
#endif
              }

              // MAC0 ----------------------------------
              valueString = String(broadcastAddress2[0], HEX); // Read current value
              // client.println("<p>Custom MAC: "); // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr2Mac0change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr2Mac0change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr2Mac0Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr2Mac0Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress2[0] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC1 ----------------------------------
              valueString = String(broadcastAddress2[1], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr2Mac1change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr2Mac1change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr2Mac1Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr2Mac1Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress2[1] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC2 ----------------------------------
              valueString = String(broadcastAddress2[2], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr2Mac2change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr2Mac2change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr2Mac2Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr2Mac2Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress2[2] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC3 ----------------------------------
              valueString = String(broadcastAddress2[3], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr2Mac3change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr2Mac3change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr2Mac3Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr2Mac3Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress2[3] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC4 ----------------------------------
              valueString = String(broadcastAddress2[4], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr2Mac4change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr2Mac4change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr2Mac4Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr2Mac4Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress2[4] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC5 ----------------------------------
              valueString = String(broadcastAddress2[5], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr2Mac5change(this.value)\" value=\"" + valueString + "\" /></p>"); // Set new value
              client.println("<script> function Tr2Mac5change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr2Mac5Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr2Mac5Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress2[5] = strtol(valueString.c_str(), NULL, 16);
              }

              // Trailer 3 ********************************************************************************************************
              // use trailer ----------------------------------
              if (useTrailer3 == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr3\" checked onclick=\"CheckboxTr3Change(this.checked)\"> use trailer 3: </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr3\" unchecked onclick=\"CheckboxTr3Change(this.checked)\"> use trailer 3: </input></p>");
              }

              client.println("<script> function CheckboxTr3Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?CheckboxTr3=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?CheckboxTr3=true") >= 0)
              {
                useTrailer3 = true;
                Serial.println("Trailer 3 enabled");
#if defined ENABLE_WIRELESS
                if (!esp_now_is_peer_exist(broadcastAddress3))
                {
                  esp_now_peer_info_t p = {};
                  memcpy(p.peer_addr, broadcastAddress3, 6);
                  p.channel = 0;
                  p.encrypt = false;
                  esp_now_add_peer(&p);
                }
#endif
              }
              else if (header.indexOf("GET /?CheckboxTr3=false") >= 0)
              {
                useTrailer3 = false;
                Serial.println("Trailer 3 disabled");
#if defined ENABLE_WIRELESS
                if (esp_now_is_peer_exist(broadcastAddress3))
                  esp_now_del_peer(broadcastAddress3);
#endif
              }

              // MAC0 ----------------------------------
              valueString = String(broadcastAddress3[0], HEX); // Read current value
              // client.println("<p>Custom MAC: "); // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr3Mac0change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr3Mac0change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr3Mac0Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr3Mac0Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress3[0] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC1 ----------------------------------
              valueString = String(broadcastAddress3[1], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr3Mac1change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr3Mac1change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr3Mac1Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr3Mac1Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress3[1] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC2 ----------------------------------
              valueString = String(broadcastAddress3[2], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr3Mac2change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr3Mac2change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr3Mac2Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr3Mac2Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress3[2] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC3 ----------------------------------
              valueString = String(broadcastAddress3[3], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr3Mac3change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr3Mac3change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr3Mac3Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr3Mac3Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress3[3] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC4 ----------------------------------
              valueString = String(broadcastAddress3[4], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr3Mac4change(this.value)\" value=\"" + valueString + "\" />"); // Set new value (no </p> = no new line)
              client.println("<script> function Tr3Mac4change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr3Mac4Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr3Mac4Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress3[4] = strtol(valueString.c_str(), NULL, 16);
              }

              // MAC5 ----------------------------------
              valueString = String(broadcastAddress3[5], HEX); // Read current value
              client.println(":");                             // Display title

              client.println("<input type=\"text\" style=\"text-transform: uppercase\" size=\"2\" maxlength=\"2\" class=\"textbox\" oninput=\"Tr3Mac5change(this.value)\" value=\"" + valueString + "\" /></p>"); // Set new value
              client.println("<script> function Tr3Mac5change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Tr3Mac5Set=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Tr3Mac5Set=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                broadcastAddress3[5] = strtol(valueString.c_str(), NULL, 16);
              }

              client.printf("<p>Use HEX values (0-9, A-F) only, always starting with FE");

              client.println("</div>");

              client.println("<hr>"); // ESC settings ===================================================================================================================================================
              client.println("<button type=\"button\" class=\"collapsible\">ESC settings</button>");
              client.println("<div class=\"content\">");
              client.printf("<p style=\"color:red;\"><b>Lift traction wheels off the ground while adjusting!</b></p>");

              // Slider1 (ESC pulse span) ----------------------------------
              valueString = String(escPulseSpan, DEC);
              client.println("<p>ESC pulse span (vehicle top speed, 500 = fastest, 1200 = slowest): <span id=\"textSlider1Value\">" + valueString + "</span><br>"); // Label
              client.println("<input type=\"range\" min=\"500\" max=\"1200\" step=\"50\" class=\"slider\" id=\"Slider1Input\" onchange=\"Slider1Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider1Change(pos) { ");
              client.println("var slider1Value = document.getElementById(\"Slider1Input\").value;");
              client.println("document.getElementById(\"textSlider1Value\").innerHTML = slider1Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider1=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider1=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                escPulseSpan = (valueString.toInt());
                setupMcpwmESC();
                Serial.println("escPulseSpan = " + String(escPulseSpan));
              }

              // Slider2 (ESC takeoff punch) ----------------------------------
              valueString = String(escTakeoffPunch, DEC);
              client.println("<p>ESC takeoff punch (additional motor force around neutral): <span id=\"textSlider2Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"0\" max=\"150\" step=\"5\" class=\"slider\" id=\"Slider2Input\" onchange=\"Slider2Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider2Change(pos) { ");
              client.println("var slider2Value = document.getElementById(\"Slider2Input\").value;");
              client.println("document.getElementById(\"textSlider2Value\").innerHTML = slider2Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider2=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider2=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                escTakeoffPunch = (valueString.toInt());
                setupMcpwmESC();
                Serial.println("escTakeoffPunch = " + String(escTakeoffPunch));
              }

              // Slider3 (ESC reverse plus) ----------------------------------
              valueString = String(escReversePlus, DEC);
              client.println("<p>ESC reverse plus (additional reverse speed / trim, power-cycle ESC!): <span id=\"textSlider3Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"0\" max=\"220\" step=\"5\" class=\"slider\" id=\"Slider3Input\" onchange=\"Slider3Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider3Change(pos) { ");
              client.println("var slider3Value = document.getElementById(\"Slider3Input\").value;");
              client.println("document.getElementById(\"textSlider3Value\").innerHTML = slider3Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider3=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider3=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                escReversePlus = (valueString.toInt());
                setupMcpwmESC();
                Serial.println("escReversePlus = " + String(escReversePlus));
              }

              // Slider4 (Crawler mode ESC ramp time) ----------------------------------
              valueString = String(crawlerEscRampTime, DEC);
              client.println("<p>Crawler mode ESC ramp time: <span id=\"textslider4Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"0\" max=\"20\" step=\"2\" class=\"slider\" id=\"Slider4Input\" onchange=\"Slider4Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider4Change(pos) { ");
              client.println("var slider4Value = document.getElementById(\"Slider4Input\").value;");
              client.println("document.getElementById(\"textslider4Value\").innerHTML = slider4Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider4=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider4=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                crawlerEscRampTime = (valueString.toInt());
                setupMcpwmESC();
                Serial.println("crawlerEscRampTime = " + String(crawlerEscRampTime));
              }

              // Slider5 (ESC global acceleration %) ----------------------------------
              valueString = String(globalAccelerationPercentage, DEC);
              client.println("<p>ESC global acceleration % (experimental): <span id=\"textslider5Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"100\" max=\"200\" step=\"10\" class=\"slider\" id=\"Slider5Input\" onchange=\"Slider5Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider5Change(pos) { ");
              client.println("var slider5Value = document.getElementById(\"Slider5Input\").value;");
              client.println("document.getElementById(\"textslider5Value\").innerHTML = slider5Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider5=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider5=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                globalAccelerationPercentage = (valueString.toInt());
                setupMcpwmESC();
                Serial.println("globalAccelerationPercentage = " + String(globalAccelerationPercentage));
              }

              // RZ7886 ESC only options:
#if defined RZ7886_DRIVER_MODE
              // Slider6 (RZ7886 ESC brake margin) ----------------------------------
              valueString = String(brakeMargin, DEC);
              client.println("<p>RZ7886 ESC brake margin (experimental): <span id=\"textslider6Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"0\" max=\"20\" step=\"2\" class=\"slider\" id=\"Slider6Input\" onchange=\"Slider6Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider6Change(pos) { ");
              client.println("var slider6Value = document.getElementById(\"Slider6Input\").value;");
              client.println("document.getElementById(\"textslider6Value\").innerHTML = slider6Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider6=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider6=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                brakeMargin = (valueString.toInt());
                setupMcpwmESC();
                Serial.println("brakeMargin = " + String(brakeMargin));
              }

              // Slider7 (RZ7886 ESC frequency) ----------------------------------
              valueString = String(RZ7886_FREQUENCY, DEC);
              client.println("<p>RZ7886 ESC frequency (frequencies > 500 may overheat driver!): <span id=\"textslider7Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"200\" max=\"1000\" step=\"50\" class=\"slider\" id=\"Slider7Input\" onchange=\"Slider7Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider7Change(pos) { ");
              client.println("var slider7Value = document.getElementById(\"Slider7Input\").value;");
              client.println("document.getElementById(\"textslider7Value\").innerHTML = slider7Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider7=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider7=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                RZ7886_FREQUENCY = (valueString.toInt());
                setupMcpwmESC();
                Serial.println("RZ7886_FREQUENCY = " + String(RZ7886_FREQUENCY));
              }

              // Slider8 (RZ7886 ESC dragbrake duty %) ----------------------------------
              valueString = String(RZ7886_DRAGBRAKE_DUTY, DEC);
              client.println("<p>RZ7886 ESC dragbrake % (more = stronger brake): <span id=\"textslider8Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"0\" max=\"100\" step=\"5\" class=\"slider\" id=\"Slider8Input\" onchange=\"Slider8Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider8Change(pos) { ");
              client.println("var slider8Value = document.getElementById(\"Slider8Input\").value;");
              client.println("document.getElementById(\"textslider8Value\").innerHTML = slider8Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider8=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider8=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                RZ7886_DRAGBRAKE_DUTY = (valueString.toInt());
                setupMcpwmESC();
                Serial.println("RZ7886_DRAGBRAKE_DUTY = " + String(RZ7886_DRAGBRAKE_DUTY));
              }
#endif
              client.println("</div>");

              client.println("<hr>"); // Servo settings ===================================================================================================================================================
              client.println("<button type=\"button\" class=\"collapsible\">Servo settings</button>");
              client.println("<div class=\"content\">");

              // Slider11 (Steering position left) ----------------------------------
              valueString = String(CH1L, DEC);
              client.println("<p>Steering position left: <span id=\"textslider11Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"800\" max=\"2200\" step=\"10\" class=\"slider sliderServo1\" id=\"Slider11Input\" onchange=\"Slider11Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider11Change(pos) { ");
              client.println("var slider11Value = document.getElementById(\"Slider11Input\").value;");
              client.println("document.getElementById(\"textslider11Value\").innerHTML = slider11Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider11=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider11=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                CH1L = (valueString.toInt());
                setupMcpwm();
                Serial.println("CH1L = " + String(CH1L));
              }

              // Slider12 (Steering position center) ----------------------------------
              valueString = String(CH1C, DEC);
              client.println("<p>Steering position center: <span id=\"textslider12Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"1300\" max=\"1700\" step=\"5\" class=\"slider sliderServo1\" id=\"Slider12Input\" onchange=\"Slider12Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider12Change(pos) { ");
              client.println("var slider12Value = document.getElementById(\"Slider12Input\").value;");
              client.println("document.getElementById(\"textslider12Value\").innerHTML = slider12Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider12=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider12=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                CH1C = (valueString.toInt());
                setupMcpwm();
                Serial.println("CH1C = " + String(CH1C));
              }

              // Slider13 (Steering position right) ----------------------------------
              valueString = String(CH1R, DEC);
              client.println("<p>Steering position right: <span id=\"textslider13Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"800\" max=\"2200\" step=\"10\" class=\"slider sliderServo1\" id=\"Slider13Input\" onchange=\"Slider13Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider13Change(pos) { ");
              client.println("var slider13Value = document.getElementById(\"Slider13Input\").value;");
              client.println("document.getElementById(\"textslider13Value\").innerHTML = slider13Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider13=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider13=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                CH1R = (valueString.toInt());
                setupMcpwm();
                Serial.println("CH1R = " + String(CH1R));
              }

              // Slider14 (Transmission position left) ----------------------------------
              valueString = String(CH2L, DEC);
              client.println("<p>Transmission position left (1st gear): <span id=\"textslider14Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"800\" max=\"2200\" step=\"10\" class=\"slider sliderServo2\" id=\"Slider14Input\" onchange=\"Slider14Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider14Change(pos) { ");
              client.println("var slider14Value = document.getElementById(\"Slider14Input\").value;");
              client.println("document.getElementById(\"textslider14Value\").innerHTML = slider14Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider14=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider14=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                CH2L = (valueString.toInt());
                setupMcpwm();
                Serial.println("CH2L = " + String(CH2L));
              }

              // Slider15 (Transmission position center) ----------------------------------
              valueString = String(CH2C, DEC);
              client.println("<p>Transmission position center (2nd gear): <span id=\"textslider15Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"800\" max=\"2200\" step=\"10\" class=\"slider sliderServo2\" id=\"Slider15Input\" onchange=\"Slider15Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider15Change(pos) { ");
              client.println("var slider15Value = document.getElementById(\"Slider15Input\").value;");
              client.println("document.getElementById(\"textslider15Value\").innerHTML = slider15Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider15=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider15=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                CH2C = (valueString.toInt());
                setupMcpwm();
                Serial.println("CH2C = " + String(CH2C));
              }

              // Slider16 (Transmission position right) ----------------------------------
              valueString = String(CH2R, DEC);
              client.println("<p>Transmission position right (3rd gear): <span id=\"textslider16Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"800\" max=\"2200\" step=\"10\" class=\"slider sliderServo2\" id=\"Slider16Input\" onchange=\"Slider16Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider16Change(pos) { ");
              client.println("var slider16Value = document.getElementById(\"Slider16Input\").value;");
              client.println("document.getElementById(\"textslider16Value\").innerHTML = slider16Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider16=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider16=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                CH2R = (valueString.toInt());
                setupMcpwm();
                Serial.println("CH2R = " + String(CH2R));
              }

              // Slider17 (coupler position left) ----------------------------------
              valueString = String(CH4L, DEC);
              client.println("<p>coupler position left (locked): <span id=\"textslider17Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"1000\" max=\"2000\" step=\"10\" class=\"slider sliderServo3\" id=\"Slider17Input\" onchange=\"Slider17Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider17Change(pos) { ");
              client.println("var slider17Value = document.getElementById(\"Slider17Input\").value;");
              client.println("document.getElementById(\"textslider17Value\").innerHTML = slider17Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider17=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider17=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                CH4L = (valueString.toInt());
                setupMcpwm();
                Serial.println("CH4L = " + String(CH4L));
              }

              // Slider18 (coupler position right) ----------------------------------
              valueString = String(CH4R, DEC);
              client.println("<p>coupler position right (unlocked): <span id=\"textslider18Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"1000\" max=\"2000\" step=\"10\" class=\"slider sliderServo3\" id=\"Slider18Input\" onchange=\"Slider18Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider18Change(pos) { ");
              client.println("var slider18Value = document.getElementById(\"Slider18Input\").value;");
              client.println("document.getElementById(\"textslider18Value\").innerHTML = slider18Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider18=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider18=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                CH4R = (valueString.toInt());
                setupMcpwm();
                Serial.println("CH4R = " + String(CH4R));
              }
              client.println("</div>");

              client.println("<hr>"); // Light settings ===================================================================================================================================================
              client.println("<button type=\"button\" class=\"collapsible\">Light settings</button>");
              client.println("<div class=\"content\">");

              // Checkbox21 (Flickering while cranking) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (flickeringWileCranking == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr21\" checked onclick=\"CheckboxTr21Change(this.checked)\"> Flickering while cranking </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr21\" unchecked onclick=\"CheckboxTr21Change(this.checked)\"> Flickering while cranking </input></p>");
              }
              client.println("<script> function CheckboxTr21Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?CheckboxTr21=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?CheckboxTr21=true") >= 0)
              {
                flickeringWileCranking = true;
                Serial.println("Flickering while cranking enabled");
              }
              else if (header.indexOf("GET /?CheckboxTr21=false") >= 0)
              {
                flickeringWileCranking = false;
                Serial.println("Flickering while cranking disabled");
              }
              client.println("</div>");

              // Checkbox22 (Xenon simulation) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (xenonLights == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr22\" checked onclick=\"CheckboxTr22Change(this.checked)\"> Xenon simulation </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr22\" unchecked onclick=\"CheckboxTr22Change(this.checked)\"> Xenon simulation </input></p>");
              }
              client.println("<script> function CheckboxTr22Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?CheckboxTr22=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?CheckboxTr22=true") >= 0)
              {
                xenonLights = true;
                Serial.println("Xenon simulation enabled");
              }
              else if (header.indexOf("GET /?CheckboxTr22=false") >= 0)
              {
                xenonLights = false;
                Serial.println("Xenon simulation disabled");
              }
              client.println("</div>");
              // Checkbox23 (Swap L & R indicators) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (swap_L_R_indicators == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr23\" checked onclick=\"CheckboxTr23Change(this.checked)\"> Swap L & R indicators </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr23\" unchecked onclick=\"CheckboxTr23Change(this.checked)\"> Swap L & R indicators </input></p>");
              }
              client.println("<script> function CheckboxTr23Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?CheckboxTr23=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?CheckboxTr23=true") >= 0)
              {
                swap_L_R_indicators = true;
                Serial.println("Swap L & R indicators enabled");
              }
              else if (header.indexOf("GET /?CheckboxTr23=false") >= 0)
              {
                swap_L_R_indicators = false;
                Serial.println("Swap L & R indicators disabled");
              }
              client.println("</div>");

              // Checkbox24 (Indicators as sidemarkers) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (indicatorsAsSidemarkers == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr24\" checked onclick=\"CheckboxTr24Change(this.checked)\"> Indicators as sidemarkers </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr24\" unchecked onclick=\"CheckboxTr24Change(this.checked)\"> Indicators as sidemarkers </input></p>");
              }
              client.println("<script> function CheckboxTr24Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?CheckboxTr24=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?CheckboxTr24=true") >= 0)
              {
                indicatorsAsSidemarkers = true;
                Serial.println("Indicators as sidemarkers enabled");
              }
              else if (header.indexOf("GET /?CheckboxTr24=false") >= 0)
              {
                indicatorsAsSidemarkers = false;
                Serial.println("Indicators as sidemarkers disabled");
              }
              client.println("</div>");

              // Checkbox28 (LED indicators) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (ledIndicators == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr28\" checked onclick=\"Checkboxtr28Change(this.checked)\"> LED indicators </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr28\" unchecked onclick=\"Checkboxtr28Change(this.checked)\"> LED indicators </input></p>");
              }
              client.println("<script> function Checkboxtr28Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Checkboxtr28=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Checkboxtr28=true") >= 0)
              {
                ledIndicators = true;
                Serial.println("LED indicators enabled");
              }
              else if (header.indexOf("GET /?Checkboxtr28=false") >= 0)
              {
                ledIndicators = false;
                Serial.println("LED indicators disabled");
              }
              client.println("</div>");

              // Checkbox25 (Separate full beam) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (separateFullBeam == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr25\" checked onclick=\"CheckboxTr25Change(this.checked)\">Separate full beam (roof light connector)</input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr25\" unchecked onclick=\"CheckboxTr25Change(this.checked)\">Separate full beam (roof light connector)</input></p>");
              }
              client.println("<script> function CheckboxTr25Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?CheckboxTr25=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?CheckboxTr25=true") >= 0)
              {
                separateFullBeam = true;
                Serial.println("Separate full beam enabled");
              }
              else if (header.indexOf("GET /?CheckboxTr25=false") >= 0)
              {
                separateFullBeam = false;
                Serial.println("Separate full beam disabled");
              }
              client.println("</div>");

              // Checkbox26 (No cab lights) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (noCabLights == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr26\" checked onclick=\"Checkboxtr26Change(this.checked)\"> No cab lights </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr26\" unchecked onclick=\"Checkboxtr26Change(this.checked)\"> No cab lights </input></p>");
              }
              client.println("<script> function Checkboxtr26Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Checkboxtr26=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Checkboxtr26=true") >= 0)
              {
                noCabLights = true;
                Serial.println("No cab lights enabled");
              }
              else if (header.indexOf("GET /?Checkboxtr26=false") >= 0)
              {
                noCabLights = false;
                Serial.println("No cab lights disabled");
              }
              client.println("</div>");

              // Checkbox27 (No fog lights) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (noFogLights == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr27\" checked onclick=\"Checkboxtr27Change(this.checked)\"> No fog lights </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr27\" unchecked onclick=\"Checkboxtr27Change(this.checked)\"> No fog lights </input></p>");
              }
              client.println("<script> function Checkboxtr27Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Checkboxtr27=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Checkboxtr27=true") >= 0)
              {
                noFogLights = true;
                Serial.println("No fog lights enabled");
              }
              else if (header.indexOf("GET /?Checkboxtr27=false") >= 0)
              {
                noFogLights = false;
                Serial.println("No fog lights disabled");
              }
              client.println("</div>");

              // Checkbox29 (Flashing blue lights) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (flashingBlueLight == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr29\" checked onclick=\"Checkboxtr29Change(this.checked)\"> Flashing blue lights </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr29\" unchecked onclick=\"Checkboxtr29Change(this.checked)\"> Flashing blue lights </input></p>");
              }
              client.println("<script> function Checkboxtr29Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Checkboxtr29=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Checkboxtr29=true") >= 0)
              {
                flashingBlueLight = true;
                Serial.println("Flashing blue lights enabled");
              }
              else if (header.indexOf("GET /?Checkboxtr29=false") >= 0)
              {
                flashingBlueLight = false;
                Serial.println("Flashing blue lights disabled");
              }
              client.println("</div>");

              // Checkbox30 (Hazards on, if 5th wheel is unlocked) ----------------------------------
              client.println("<div class=\"multiColumn\">");
              if (hazardsWhile5thWheelUnlocked == true)
              {
                client.println("<p><input type=\"checkbox\" id=\"tr30\" checked onclick=\"Checkboxtr30Change(this.checked)\"> Hazards on, if 5th wheel is unlocked </input></p>");
              }
              else
              {
                client.println("<p><input type=\"checkbox\" id=\"tr30\" unchecked onclick=\"Checkboxtr30Change(this.checked)\"> Hazards on, if 5th wheel is unlocked </input></p>");
              }
              client.println("<script> function Checkboxtr30Change(pos) { ");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Checkboxtr30=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Checkboxtr30=true") >= 0)
              {
                hazardsWhile5thWheelUnlocked = true;
                Serial.println("Hazards on, if 5th wheel is unlocked enabled");
              }
              else if (header.indexOf("GET /?Checkboxtr30=false") >= 0)
              {
                hazardsWhile5thWheelUnlocked = false;
                Serial.println("Hazards on, if 5th wheel is unlocked disabled");
              }
              client.println("</div>");

              // Dummy checkbox
              //client.println("<div class=\"multiColumn\">");
              //client.println("</div>");

              /*
              uint8_t cabLightsBrightness = 100;      // Usually 255, 100 for Actros & Ural
              uint8_t sideLightsBrightness = 150;     // Usually 200, 100 for WPL C44, 50 for Landy, 100 for P407, 150 for Actros
              uint8_t reversingLightBrightness = 140; // Around 140, 50 for Landy & Ural
              uint8_t fogLightBrightness = 200;       // Around 200
              uint8_t rearlightDimmedBrightness = 30; // tailligt brightness, if not braking, about 30
              uint8_t rearlightParkingBrightness = 3; // 0, if you want the taillights being off, if side lights are on, or about 5 if you want them on (0 for US Mode)
              uint8_t headlightParkingBrightness = 3; // 0, if you want the headlights being off, if side lights are on, or about 5 if you want them on (0 for US Mode)
              */

              // Slider21 (Cab light brightness) ----------------------------------
              valueString = String(cabLightsBrightness, DEC);
              client.println("<p>Cab light brightness: <span id=\"textslider21Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"50\" max=\"255\" step=\"5\" class=\"slider sliderLed\" id=\"Slider21Input\" onchange=\"Slider21Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider21Change(pos) { ");
              client.println("var slider21Value = document.getElementById(\"Slider21Input\").value;");
              client.println("document.getElementById(\"textslider21Value\").innerHTML = slider21Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider21=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider21=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                cabLightsBrightness = (valueString.toInt());
                Serial.println("cabLightsBrightness = " + String(cabLightsBrightness));
              }

              // Slider22 (Side light brightness) ----------------------------------
              valueString = String(sideLightsBrightness, DEC);
              client.println("<p>Side light brightness: <span id=\"textslider22Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"50\" max=\"255\" step=\"5\" class=\"slider sliderLed\" id=\"Slider22Input\" onchange=\"Slider22Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider22Change(pos) { ");
              client.println("var slider22Value = document.getElementById(\"Slider22Input\").value;");
              client.println("document.getElementById(\"textslider22Value\").innerHTML = slider22Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider22=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider22=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                sideLightsBrightness = (valueString.toInt());
                Serial.println("sideLightsBrightness = " + String(sideLightsBrightness));
              }

              // Slider23 (Reversing light brightness) ----------------------------------
              valueString = String(reversingLightBrightness, DEC);
              client.println("<p>Reversing light brightness: <span id=\"textslider23Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"50\" max=\"255\" step=\"5\" class=\"slider sliderLed\" id=\"Slider23Input\" onchange=\"Slider23Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider23Change(pos) { ");
              client.println("var slider23Value = document.getElementById(\"Slider23Input\").value;");
              client.println("document.getElementById(\"textslider23Value\").innerHTML = slider23Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider23=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider23=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                reversingLightBrightness = (valueString.toInt());
                Serial.println("reversingLightBrightness = " + String(reversingLightBrightness));
              }

              // Slider27 (Fog light brightness) ----------------------------------
              valueString = String(fogLightBrightness, DEC);
              client.println("<p>Fog light brightness: <span id=\"textslider27Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"100\" max=\"255\" step=\"5\" class=\"slider sliderLed\" id=\"Slider27Input\" onchange=\"Slider27Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider27Change(pos) { ");
              client.println("var slider27Value = document.getElementById(\"Slider27Input\").value;");
              client.println("document.getElementById(\"textslider27Value\").innerHTML = slider27Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider27=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider27=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                fogLightBrightness = (valueString.toInt());
                Serial.println("fogLightBrightness = " + String(fogLightBrightness));
              }

              // Slider24 (Tail light dimmed brightness while not braking) ----------------------------------
              valueString = String(rearlightDimmedBrightness, DEC);
              client.println("<p>Tail light dimmed brightness (while not braking, use low value in Indicators as sidemarkers mode): <span id=\"textslider24Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"30\" max=\"255\" step=\"5\" class=\"slider sliderLed\" id=\"Slider24Input\" onchange=\"Slider24Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider24Change(pos) { ");
              client.println("var slider24Value = document.getElementById(\"Slider24Input\").value;");
              client.println("document.getElementById(\"textslider24Value\").innerHTML = slider24Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider24=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider24=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                rearlightDimmedBrightness = (valueString.toInt());
                Serial.println("rearlightDimmedBrightness = " + String(rearlightDimmedBrightness));
              }

              // Slider25 (Tail light dimmed brightness while parking lights only) ----------------------------------
              valueString = String(rearlightParkingBrightness, DEC);
              client.println("<p>Tail light dimmed brightness (while parking lights only): <span id=\"textslider25Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"0\" max=\"5\" step=\"1\" class=\"slider sliderLed\" id=\"Slider25Input\" onchange=\"Slider25Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider25Change(pos) { ");
              client.println("var slider25Value = document.getElementById(\"Slider25Input\").value;");
              client.println("document.getElementById(\"textslider25Value\").innerHTML = slider25Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider25=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider25=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                rearlightParkingBrightness = (valueString.toInt());
                Serial.println("rearlightParkingBrightness = " + String(rearlightParkingBrightness));
              }

              // Slider26 (Head light dimmed brightness while parking lights only) ----------------------------------
              valueString = String(headlightParkingBrightness, DEC);
              client.println("<p>Head light dimmed brightness (while parking lights only): <span id=\"textslider26Value\">" + valueString + "</span><br>");
              client.println("<input type=\"range\" min=\"0\" max=\"5\" step=\"1\" class=\"slider sliderLed\" id=\"Slider26Input\" onchange=\"Slider26Change(this.value)\" value=\"" + valueString + "\" /></p>");
              client.println("<script> function Slider26Change(pos) { ");
              client.println("var slider26Value = document.getElementById(\"Slider26Input\").value;");
              client.println("document.getElementById(\"textslider26Value\").innerHTML = slider26Value;");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Slider26=\" + pos + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Slider26=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                headlightParkingBrightness = (valueString.toInt());
                Serial.println("headlightParkingBrightness = " + String(headlightParkingBrightness));
              }

              // Select Neopixel animation mode -------------------------------------------------------------------------------

              client.println("<label for=\"select1Input\">Neopixel animation mode:</label>");
              client.println("<select name=\"select1\" class=\"button\" id=\"select1Input\" onchange=\"select1Change(this)\">");
              client.println("<option value=\"1\">1 = Demo (don't use it)</option>");
              client.println("<option value=\"2\">2 = Knight Rider scanner animation for 8 LED </option>");
              client.println("<option value=\"3\">3 = Bluelight animation for 8 LED </option>");
              client.println("<option value=\"4\">4 = Union Jack United Kingdom animation for 8 LED</option>");
              client.println("<option value=\"5\">5 = B33lz3bub Austria animation for 3 LED</option>");
              client.println("</select>");

              client.println("<script> function select1Change(pos) { ");
              client.println("var select1Value = pos.value;"); // OK
              client.println("console.log(select1Value);");
              client.println("var xhr = new XMLHttpRequest();");
              client.println("xhr.open('GET', \"/?Select1=\" + select1Value + \"&\", true);");
              client.println("xhr.send(); } </script>");

              if (header.indexOf("GET /?Select1=") >= 0)
              {
                pos1 = header.indexOf('=');
                pos2 = header.indexOf('&');
                valueString = header.substring(pos1 + 1, pos2);
                neopixelMode = (valueString.toInt());
                setupNeopixel();
                Serial.println("neopixelMode = " + String(neopixelMode));
              }

              client.println("</div>");

              client.println("<hr>"); // Horizontal line ===================================================================================================================================================

              client.printf("<p>Save settings & restart required after changes above");

              // button1 (Save settings to EEPROM) ----------------------------------
              client.println("<p><a href=\"/save/on\"><button class=\"button buttonRed\" onclick=\"restartPopup()\" >Save settings & restart</button></a></p>");

              if (header.indexOf("GET /save/on") >= 0)
              {
                eepromWrite();
                delay(1000);
                ESP.restart();
              }
              client.println("<script> function restartPopup() {");
              client.println("alert(\"Controller restarted, you may need to reconnect WiFi!\"); ");
              client.println("} </script>");

              client.println("<p>It is recommended to power-cycle the controller as well as to close and reopen the browser window after using this button!</p>");

              //-----------------------------------------------------------------------------------------------------------------------

              client.println("<br>More informations on my <a href=\"https://thediyguy999.github.io/TheDIYGuy999_ESP32_Web_Flasher/index.html\" target=\"_blank\">Website</a><br>");

              //-----------------------------------------------------------------------------------------------------------------------
              // Script for collapsible sections
              client.println("<script> var coll = document.getElementsByClassName(\"collapsible\"); var i; for (i = 0; i < coll.length; i++) { coll[i].addEventListener(\"click\", function() { this.classList.toggle(\"active\"); var content = this.nextElementSibling; if (content.style.display === \"block\") { content.style.display = \"none\"; } else { content.style.display = \"block\"; } }); } </script>");

              // Script for reading defaults
              client.println("<script> function readDefaults() {");
              client.println("document.getElementById(\"select1Input\").value = \"" + String(neopixelMode, DEC) + "\";");
              client.println("}</script>");

              //-----------------------------------------------------------------------------------------------------------------------

              client.println("</body></html>");

              // The HTTP-response ends with an empty column
              client.println();
              // Break out of the while loop
              break;
            }
            else
            { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          }
          else if (c != '\r')
          {                   // if you got anything else but a carriage return character,
            currentLine += c; // add it to the end of the currentLine
          }
        }
      }
      // Clear header
      header = "";
      // Disconnect client
      client.stop();
      Serial.println("Client disconnected.");

#if defined DEBUG
      Serial.print("DEBUG stuff: ");
      // Serial.printf("Trailer 1 MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", broadcastAddress1[0], broadcastAddress1[1], broadcastAddress1[2], broadcastAddress1[3], broadcastAddress1[4], broadcastAddress1[5]);
      Serial.println("");
#endif
    }
  }
}

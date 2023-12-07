
//Flash memory___________________________________________________________________________________________

#include "StringSplitter.h"
#include "nvs_flash.h"
#include "nvs.h"

//Mail______________________________________________
#include <ESP_Mail_Client.h>
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "pressuremonitorlog@gmail.com"
#define AUTHOR_PASSWORD "prue yuer ithb cqqk"

String recipient0;
String recipient1;
String recipient2;
String recipient3;
String recipient4;


bool mailSent;

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void sendMail(){
  if(!mailSent){
    loadingBar(10,30);
    MailClient.networkReconnect(true);  
    smtp.debug(1);
    smtp.callback(smtpCallback);
    Session_Config config;
    config.server.host_name = SMTP_HOST;
    config.server.port = SMTP_PORT;
    config.login.email = AUTHOR_EMAIL;
    config.login.password = AUTHOR_PASSWORD;
    config.login.user_domain = "";

    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = (BPMSID+" ("+area+")");
    message.sender.email = AUTHOR_EMAIL;
    message.subject = F("Low pressure alert");
    message.addRecipient("", AUTHOR_EMAIL);

    if(!recipient0.equals("")){
      char chrRecipient0[100];
      recipient0.toCharArray(chrRecipient0, sizeof(chrRecipient0));
      message.addRecipient("", chrRecipient0);
      Serial.print(chrRecipient0);
      Serial.println(" Recipient successfully added");
    }if(!recipient1.equals("")){
      char chrRecipient1[100];
      recipient1.toCharArray(chrRecipient1, sizeof(chrRecipient1));
      message.addRecipient(F(""), chrRecipient1);
      Serial.print(chrRecipient1);
      Serial.println(" Recipient successfully added");
    }if(!recipient2.equals("")){
      char chrRecipient2[100];
      recipient2.toCharArray(chrRecipient2, sizeof(chrRecipient2));
      message.addRecipient(F(""), chrRecipient2);
      Serial.print(chrRecipient2);
      Serial.println(" Recipient successfully added");
    }if(!recipient3.equals("")){
      char chrRecipient3[100];
      recipient3.toCharArray(chrRecipient3, sizeof(chrRecipient3));
      message.addRecipient(F(""), chrRecipient3);
      Serial.print(chrRecipient3);
      Serial.println(" Recipient successfully added");
    }if(!recipient4.equals("")){
      char chrRecipient4[100];
      recipient4.toCharArray(chrRecipient4, sizeof(chrRecipient4));
      message.addRecipient(F(""), chrRecipient4);
      Serial.print(chrRecipient4);
      Serial.println(" Recipient successfully added");
    }
    //Send raw text message
    //String textMsg = "Pressure is below 0.9 Bars";
    //message.text.content = textMsg.c_str();
    //message.text.charSet = "us-ascii";
    //message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    
    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
    /*Send HTML message*/
    String htmlMsg = "<div style=\"font-family: 'Arial', sans-serif; background-color: rgb(248, 215, 218); border: 1px solid rgb(217, 76, 83); border-radius: 5px; padding: 15px; text-align: center;\">" 
                      "<h1 style=\"color: rgb(114, 28, 36);\">⚠ <u><b>Pressure Alert : "+area+" </b></u> ⚠</h1>" 
                      "<p style=\"color: rgb(114, 28, 36); font-size: 16px; font-weight: bold;\">PRESSURE IS CRITICALLY LOW.</p>"
                      "<p style=\"color: rgb(114, 28, 36); font-size: 16px; font-weight: bold;\">Current pressure is "+pressure+" Bars.</p>"
                      "<p style=\"color: rgb(114, 28, 36); font-size: 16px; font-weight: bold;\">Cutoff limit is "+cutoff+" Bars.</p>"
                      "Alert sent to "+recipient0+"\n"+recipient1+"\n"+recipient2+"\n"+recipient3+"\n"+recipient4+
                    "</div>";

    message.html.content = htmlMsg.c_str();
    message.html.content = htmlMsg.c_str();
    message.text.charSet = "us-ascii";
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    loadingBar(30,40);

    /* Connect to the server */
    if (!smtp.connect(&config)){
      ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
      return;
    }

    if (!smtp.isLoggedIn()){
      Serial.println("\nNot yet logged in.");
    }
    else{
      if (smtp.isAuthenticated()){
        Serial.println("\nSuccessfuly logged in.");
        loadingBar(40,80);
      }else{
        Serial.println("\nConnected with no Auth.");
      }
    }

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message))
      ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
  }
}



/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");
    loadingBar(80,118);

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
 
      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
    mailSent=true;
    return;
  }
}



String emailSplitter(String stremails){
  Serial.println("Emails Received");
  //Serial.println("Test String: " + strPayload);
  StringSplitter *splitter = new StringSplitter(stremails, '\n', 25);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
  Serial.println("Recipient count: " + String(itemCount));
  for(int i = 0; i < 5; i++){
    String item = splitter->getItemAtIndex(i);
    //Serial.println("Item @ index " + String(i) + ": " + String(item));
    if(i==0){
      recipient0=item;
      Serial.print("Recipient0 :");
      Serial.println(recipient0);
    }else if(i==1){
      recipient1=item;
      Serial.print("Recipient1 :");
      Serial.println(recipient1);
    }else if(i==2){
      recipient2=item;
      Serial.print("Recipient2 :");
      Serial.println(recipient2);      
    }else if(i==3){
      recipient3=item; 
      Serial.print("Recipient3 :");
      Serial.println(recipient3);     
    }else if(i==4){
      recipient4=item;
      Serial.print("Recipient4 :");
      Serial.println(recipient4);      
    }
  }
  return("Splitted emails sucessfully");
}



void loadingBar(int start,int end){
  for(int i=start;i<end;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
    display.display();
  }
}
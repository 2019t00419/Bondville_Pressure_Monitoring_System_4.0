
//google sheet__________________________________________________________________________
//Include required libraries
#include <HTTPClient.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

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
char* RECIPIENT_EMAIL = "sandunviduranga97@gmail.com";
String Recipients[5];
/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void sendMail(){

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
  message.addRecipient(F("System"), "pressuremonitorlog@gmail.com");
  message.addRecipient(F("Admin"), RECIPIENT_EMAIL);
  //Send raw text message
  //String textMsg = "Pressure is below 0.9 Bars";
  //message.text.content = textMsg.c_str();
  //message.text.charSet = "us-ascii";
  //message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
  /*Send HTML message*/
 String htmlMsg = "<div style=\"font-family: 'Arial', sans-serif; background-color: rgb(248, 215, 218); border: 1px solid rgb(217, 76, 83); border-radius: 5px; padding: 15px; text-align: center;\">" 
                    "<h1 style=\"color: rgb(114, 28, 36);\">⚠ CAUTION ⚠</h1>" 
                    "<p style=\"color: rgb(114, 28, 36); font-size: 16px; font-weight: bold;\">Pressure is critically low</p>" 
                  "</div>";

  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;


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
    }else{
      Serial.println("\nConnected with no Auth.");
    }
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

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
  }
}



String emailSplitter(String stremails){
  Serial.println("Emails Received");
  //Serial.println("Test String: " + strPayload);
  StringSplitter *splitter = new StringSplitter(stremails, '\n', 25);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
  //Serial.println("Item count: " + String(itemCount));

  for(int i = 0; i < itemCount; i++){
    String item = splitter->getItemAtIndex(i);
    //Serial.println("Item @ index " + String(i) + ": " + String(item));
    Recipients[i]=item;;
  }
  for(int i=0; (i>=sizeof(modes) / sizeof(modes[0]));i++){
    //Serial.print("Recipient"+i+":");
    Serial.println(Recipients[i]);
  }
}


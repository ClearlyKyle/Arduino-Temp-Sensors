
# Arduino-Temp-Sensors

ESP8266 based wireless temperature logging

Wirelessly sending data to either Google Docs or InfluxDB cloud service 

## Google Docs Setup

1. Create a Google sheet
1. Create a Google scipt and add the scipt code
    1. Update the Url in the script to the url of the Google Sheet
1. Publish the script and take note of thr url
1. Add the Scipt url to the code

## InfluxDB set up

1. Create a bucket
    >Data > Buckets > Create Bucket
1. Create a Token
    >Data > Tokens > Generate > Select your bucket
1. Click on the new Token and copy the code, should be similar to:
    >"JPOpWsp9EYb7tgW7QsN0vs4VugdgKUfiaiKFlUf1zxUZYF6g_QEF77FPeP5YH6GotW2_ylghk4KGjayRROpviQ=="
1. Change the variables: "BUCKET" and "TOKEN" in the code to the newly created bucket and token
1. Alter the varaible "data" to data required.

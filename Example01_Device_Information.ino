#include <Wire.h>
#include <SparkFun_ST25DV64KC_Arduino_Library.h>

#define SDA_PIN 8
#define SCL_PIN 9

SFE_ST25DV64KC_NDEF nfc;

int receiptNumber = 1;

void writeReceipt()
{
  char url[100];

  // For now we're using example.com.
  // Later this becomes our real Clip domain.
  sprintf(url, "example.com/receipt/%03d", receiptNumber);

  Serial.println();
  Serial.println("==============================");
  Serial.print("Preparing Receipt #");
  Serial.printf("%03d\n", receiptNumber);
  Serial.println("==============================");

  Serial.print("NFC URL: https://");
  Serial.println(url);

  if (nfc.writeNDEFURI(
        url,
        SFE_ST25DV_NDEF_URI_ID_CODE_HTTPS))
  {
    Serial.println("NFC UPDATED SUCCESSFULLY!");
    Serial.println("You can tap your phone now.");
  }
  else
  {
    Serial.println("ERROR: NFC update failed!");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("     CLIP PROTOTYPE #001");
  Serial.println("     DYNAMIC RECEIPTS");
  Serial.println("==============================");
  Serial.println();

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Connecting to NFC tag...");

  if (!nfc.begin(Wire))
  {
    Serial.println("ERROR: ST25DV64KC not detected!");

    while (1)
    {
      delay(1000);
    }
  }

  Serial.println("ST25DV64KC detected!");

  // Prepare tag for NDEF
  if (!nfc.writeCCFile8Byte())
  {
    Serial.println("ERROR: Could not write CC file!");
    return;
  }

  Serial.println("NDEF memory ready.");

  // Write first receipt
  writeReceipt();

  Serial.println();
  Serial.println("------------------------------");
  Serial.println("Type n and press ENTER");
  Serial.println("to simulate the NEXT customer.");
  Serial.println("------------------------------");
}

void loop()
{
  if (Serial.available())
  {
    char command = Serial.read();

    if (command == 'n' || command == 'N')
    {
      receiptNumber++;

      writeReceipt();

      Serial.println();
      Serial.println("Type n for next receipt.");
    }
  }
}
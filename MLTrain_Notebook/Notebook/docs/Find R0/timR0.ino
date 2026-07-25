/*#include <MQUnifiedsensor.h>
#include <math.h>

//================ ESP32 =================
#define Board               "ESP-32"
#define Voltage_Resolution  3.3
#define ADC_Bit_Resolution  12

//========= GPIO ADC ==========
#define MQ2_PIN 25
#define MQ3_PIN 34
#define MQ4_PIN 35

//========= Clean Air Ratio ==========
#define RatioMQ2CleanAir 9.83
#define RatioMQ3CleanAir 60.0
#define RatioMQ4CleanAir 4.4

//========= Sensors ==========
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ2_PIN, "MQ-2");
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ3_PIN, "MQ-3");
MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ4_PIN, "MQ-4");

//=========== Settings ===========
const int SAMPLE_COUNT = 1200;
const int SAMPLE_DELAY = 500;
const unsigned long WARMUP_TIME = 600000UL; //10 phút

float sum2 = 0, sum3 = 0, sum4 = 0;
int valid2 = 0, valid3 = 0, valid4 = 0;

void setup()
{
    Serial.begin(115200);

    MQ2.init();
    MQ3.init();
    MQ4.init();

    Serial.println();
    Serial.println("====================================");
    Serial.println("      MQ SENSOR CALIBRATION");
    Serial.println("====================================");
    Serial.println("Dat thiet bi trong phong sach.");
    Serial.println("Khong thao tac vao cam bien.");
    Serial.println();

    Serial.println("Warm-up 10 phut...");
    delay(WARMUP_TIME);

    Serial.println("Bat dau calibrate...");

    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        MQ2.update();
        MQ3.update();
        MQ4.update();

        float r02 = MQ2.calibrate(RatioMQ2CleanAir);
        float r03 = MQ3.calibrate(RatioMQ3CleanAir);
        float r04 = MQ4.calibrate(RatioMQ4CleanAir);

        if (!isnan(r02) && !isinf(r02) && r02 > 0)
        {
            sum2 += r02;
            valid2++;
        }

        if (!isnan(r03) && !isinf(r03) && r03 > 0)
        {
            sum3 += r03;
            valid3++;
        }

        if (!isnan(r04) && !isinf(r04) && r04 > 0)
        {
            sum4 += r04;
            valid4++;
        }

        if ((i + 1) % 60 == 0)
        {
            Serial.print("Tien do: ");
            Serial.print((i + 1) * 100 / SAMPLE_COUNT);
            Serial.println("%");
        }

        delay(SAMPLE_DELAY);
    }

    if (valid2 == 0 || valid3 == 0 || valid4 == 0)
    {
        Serial.println();
        Serial.println("Calibration FAILED!");
        Serial.println("Khong thu duoc du lieu hop le.");
        while (true);
    }

    float R0_MQ2 = sum2 / valid2;
    float R0_MQ3 = sum3 / valid3;
    float R0_MQ4 = sum4 / valid4;

    MQ2.setR0(R0_MQ2);
    MQ3.setR0(R0_MQ3);
    MQ4.setR0(R0_MQ4);

    Serial.println();
    Serial.println("========== RESULT ==========");

    Serial.print("MQ2 R0 = ");
    Serial.print(R0_MQ2, 4);
    Serial.print("    (");
    Serial.print(valid2);
    Serial.println(" samples)");

    Serial.print("MQ3 R0 = ");
    Serial.print(R0_MQ3, 4);
    Serial.print("    (");
    Serial.print(valid3);
    Serial.println(" samples)");

    Serial.print("MQ4 R0 = ");
    Serial.print(R0_MQ4, 4);
    Serial.print("    (");
    Serial.print(valid4);
    Serial.println(" samples)");

    Serial.println("============================");
    Serial.println("Calibration Finished.");
}

void loop()
{
}*/

#include <MQUnifiedsensor.h>
#include <math.h>

//================ ESP32 =================
#define Board               "ESP-32"
#define Voltage_Resolution  3.3
#define ADC_Bit_Resolution  12

//========= GPIO ADC ==========
#define MQ2_PIN 25
#define MQ3_PIN 34
#define MQ4_PIN 35

//========= Clean Air Ratio ==========
#define RatioMQ2CleanAir 9.83
#define RatioMQ3CleanAir 60.0
#define RatioMQ4CleanAir 4.4

//========= Sensors ==========
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ2_PIN, "MQ-2");
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ3_PIN, "MQ-3");
MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ4_PIN, "MQ-4");

//=========== Settings ===========
const int SAMPLE_COUNT = 600;      // 600 x 500 ms = 5 phút
const int SAMPLE_DELAY = 500;

float sum2 = 0, sum3 = 0, sum4 = 0;
int valid2 = 0, valid3 = 0, valid4 = 0;

void setup()
{
    Serial.begin(115200);

    MQ2.init();
    MQ3.init();
    MQ4.init();

    Serial.println();
    Serial.println("====================================");
    Serial.println(" MQ SENSOR CALIBRATION");
    Serial.println("====================================");
    Serial.println("Thiet bi da warm-up san.");
    Serial.println("Bat dau lay mau 5 phut...");
    Serial.println();

    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        MQ2.update();
        MQ3.update();
        MQ4.update();

        float r02 = MQ2.calibrate(RatioMQ2CleanAir);
        float r03 = MQ3.calibrate(RatioMQ3CleanAir);
        float r04 = MQ4.calibrate(RatioMQ4CleanAir);

        if (!isnan(r02) && !isinf(r02) && r02 > 0)
        {
            sum2 += r02;
            valid2++;
        }

        if (!isnan(r03) && !isinf(r03) && r03 > 0)
        {
            sum3 += r03;
            valid3++;
        }

        if (!isnan(r04) && !isinf(r04) && r04 > 0)
        {
            sum4 += r04;
            valid4++;
        }

        if ((i + 1) % 60 == 0)
        {
            Serial.print("Tien do: ");
            Serial.print((i + 1) * 100 / SAMPLE_COUNT);
            Serial.println("%");
        }

        delay(SAMPLE_DELAY);
    }

    if (valid2 == 0 || valid3 == 0 || valid4 == 0)
    {
        Serial.println("Calibration FAILED!");
        while (true);
    }

    float R0_MQ2 = sum2 / valid2;
    float R0_MQ3 = sum3 / valid3;
    float R0_MQ4 = sum4 / valid4;

    MQ2.setR0(R0_MQ2);
    MQ3.setR0(R0_MQ3);
    MQ4.setR0(R0_MQ4);

    Serial.println();
    Serial.println("========== RESULT ==========");

    Serial.print("MQ2 R0 = ");
    Serial.println(R0_MQ2, 4);

    Serial.print("MQ3 R0 = ");
    Serial.println(R0_MQ3, 4);

    Serial.print("MQ4 R0 = ");
    Serial.println(R0_MQ4, 4);

    Serial.println("============================");
    Serial.println("Calibration Finished.");
}

void loop()
{
}
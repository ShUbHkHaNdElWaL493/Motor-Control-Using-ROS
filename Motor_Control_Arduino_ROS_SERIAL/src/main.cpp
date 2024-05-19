#include <ros.h>
#include <std_msgs/Int64.h>

// Motor and Encoder pins
#define Direction 0
#define PWM 0
#define EncoderA 2
#define EncoderB 3

// Initializing the Node Handle
ros::NodeHandle nh;

// Class for motors with encoders for MD10C drivers
class integrated_encoder_and_motor
{

  private:

  // Motor and Encoder Pins
  uint8_t direction, pwm, encoderA, encoderB;

  // Pulses required to complete a rotation
  int pulsesPerRotation;

  void stop()
  {
    analogWrite(pwm, 0);
  }

  void rotateAntiClockwise(int speedPercentage)
  {
    digitalWrite(direction, LOW);
    analogWrite(pwm, 255 * speedPercentage / 100);
  }

  void rotateClockwise(int speedPercentage)
  {
    digitalWrite(direction, HIGH);
    analogWrite(pwm, 255 * speedPercentage / 100);
  }

  public:

  // Pulses of the encoder
  long pulseCount;

  // Constructor for this class
  integrated_encoder_and_motor(int direction, int pwm, int encoderA, int encoderB, int pulsesPerRotation)
  {
    this->direction = direction;
    this->pwm = pwm;
    this->encoderA = encoderA;
    this->encoderB = encoderB;
    this->pulsesPerRotation = pulsesPerRotation;
    this->pulseCount = 0;
  }

  // Function to rotate the motor
  void rotate(int speedPercentage)
  {
    if (speedPercentage > 0 && speedPercentage <= 100)
    {
      rotateAntiClockwise(speedPercentage);
    } else if (speedPercentage < 0 && speedPercentage >= -100)
    {
      rotateClockwise((-1) * speedPercentage);
    } else
    {
      analogWrite(pwm, 0);
    }
  }

  // Functions for changing the pulse count

  void changePulseCount0()
  {
    if (digitalRead(EncoderB) == LOW)
    {
      pulseCount++;
    } else
    {
      pulseCount--;
    }
  }

  void changePulseCount1()
  {
    if (digitalRead(EncoderA) == LOW)
    {
      pulseCount--;
    } else
    {
      pulseCount++;
    }
  }

};

// Definition of the motor and the encoder functions

integrated_encoder_and_motor Motor(Direction, PWM, EncoderA, EncoderB, 7605);

void changePulseCount_0()
{
  Motor.changePulseCount0();
}

void changePulseCount_1()
{
  Motor.changePulseCount1();
}

// Subscriber function
void receivePWM(const std_msgs::Int64 &msg)
{
  Motor.rotate(msg.data);
}

// Message to be published
std_msgs::Int64 msg;

// Subscriber and Publisher objects
ros::Subscriber<std_msgs::Int64> Subscriber_Motor("Motor", &receivePWM);
ros::Publisher Publisher_Motor("Encoder", &msg);

void setup() {

  // put your setup code here, to run once:

  // Pinmodes of the pins
  pinMode(Direction, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(EncoderA, INPUT_PULLUP);
  pinMode(EncoderB, INPUT_PULLUP);

  // Interrupts to be attached
  attachInterrupt(EncoderA, changePulseCount_0, RISING);
  attachInterrupt(EncoderB, changePulseCount_1, RISING);

  // Initializing the nodes and the subscriber object
  nh.initNode();
  nh.subscribe(Subscriber_Motor);
  nh.advertise(Publisher_Motor);

}

void loop() {

  // put your main code here, to run repeatedly:

  msg.data = Motor.pulseCount;
  Publisher_Motor.publish(&msg);

  nh.spinOnce();
  delay(1000);

}
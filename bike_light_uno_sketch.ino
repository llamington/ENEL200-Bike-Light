
int led_group_1_pin = 3; // I don't think we have access to pin 10 so I'm using 3
int led_group_2_pin = 9;
int analog_input = A0;
int last_x_values[100] = {0}; // had a hacky implementation of value averaging before which didn't really do much, this one should be better but hasn't been tested
void setup() {
  // put your setup code here, to run once:
  pinMode(led_group_1_pin, OUTPUT);
  pinMode(led_group_2_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(analog_input);
  int average_value = average_val(val, 5);
  Serial.println(val);
  Serial.println(average_value);
  analogWrite(led_group_1_pin, map(average_value, 0, 1023, 15, 220)); // my uno kept on restarting every 20 seconds when I had the max PWM set to 255, doesn't seem to happen with lower max PWM so don't know the story there
  analogWrite(led_group_2_pin, map(average_value, 0, 1023, 220, 15)); // min PWM set to 15 as I'm not sure we want the 'other' LED group fully off even at full head tilt
}

int average_val(int new_val, int num_to_average) { // shifts the recent input values along in the array (dropping the oldest) and sticks the new value in at index 0 before summing and returning the average
  int sum = 0;
  for (int i = 0; i < (num_to_average - 1); i++) {
    last_x_values[i + 1] = last_x_values[i];
  }
  last_x_values[0] = new_val;
  for (int i = 0; i < num_to_average; i++) {
    sum += last_x_values[i];
  }
  return (sum / num_to_average); // things to consider: weighted averaging (wouldn't be overly difficult to implement and could be a better option), intialisation (currently the value array is all zeros at start), integer division (do we need to bother with using a float and rounding it)
}

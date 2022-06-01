#include <ESP32Servo360.h>

int ESP32Servo360::channel_next_free = 0;

ESP32Servo360::ESP32Servo360(int offsetAngle, int rpm, int deceleration, int minPulseWidth, int maxPulseWidth, int channel)
{
    _resetFields();
    _offsetAngle = offsetAngle;
    _target = offsetAngle;
    _rpm = rpm;
    _minTorque = MIN_TORQUE;
    _minRpm = MIN_RPM;
    _minPulseWidth = minPulseWidth;
    _maxPulseWidth = maxPulseWidth;
    _updateHandle = NULL;
    _speed = 0;
    _delta = 0;

    _hold = false;
    _pwmValue = -1; // unset
    _deceleration = deceleration;
};

ESP32Servo360::~ESP32Servo360()
{
    detach();
}
/**
 * @brief Attach to the correct pins of your servo.
 * 
 * @param ctrlPin is the white cable.
 * @param feedbackPin is the yellow cable.
 * @return true 
 * @return false 
 */
bool ESP32Servo360::attach(int ctrlPin, int feedbackPin)
{

    if (_channel == CHANNEL_NOT_ATTACHED)
    {
        if (channel_next_free == CHANNEL_MAX_NUM)
        {
            return false;
        }
        else
        {
            _channel = channel_next_free;
            channel_next_free++;
        }
    }

    _ctrlPin = ctrlPin;
    _feedbackPin = feedbackPin;

    ledcSetup(_channel, 50, 16); // channel X, 50 Hz, 16-bit depth
    ledcAttachPin(_ctrlPin, _channel);

    pinMode(_feedbackPin, INPUT);
    attachInterruptArg(_feedbackPin, _isr, this, CHANGE);

    // _beginLoop();
    delay(100); // wait for first angle calculation

    _computeAngle();

    if (_angle < 0)
    {
        _angle += 360;
        _orientation = _angle;
    }
    Serial.println(_angle);

    return true;
}
/**
 * @brief If you decide to change the orientation of your motor, you can offset its zero position.
 * 
 * @param offsetAngle 
 */
void ESP32Servo360::setOffset(int offsetAngle)
{
    float trueTarget = _target - _offsetAngle;
    _offsetAngle = offsetAngle;
    _target = trueTarget + _offsetAngle;
}
/**
 * @brief To prevent any bouncing, especially due to high RPM.
 * You can start the deceleration to a higher angle from the target angle.
 * Default is set to 180 degrees.
 * 
 * @param deceleration 
 */
void ESP32Servo360::setDeceleration(int deceleration)
{
    _deceleration = deceleration;
}
/**
 * @brief For better readable angle accuracy, manually set the PWM signal of the internal Hall Effect Sensor.
 * Get the values with the method servo.calibrate();.
 * 
 * @param minPulseWidth Default minPulseWidth is 32. Signal at 0 degrees.
 * @param maxPulseWidth Default maxPulseWidth is 1067. Signal at ~360 degrees.
 */
void ESP32Servo360::adjustSignal(int minPulseWidth, int maxPulseWidth)
{
    _maxPulseWidth = constrain(maxPulseWidth, 0, 1500);
    _minPulseWidth = constrain(minPulseWidth, 0, maxPulseWidth);
}
/**
 * @brief For better readable angle accuracy, automatically set the PWM signal of the internal Hall Effect Sensor.
 * The servo will turn and give you the result through Serial.
 * 
 * @param show_origAngle 
 */
void ESP32Servo360::calibrate(int show_origAngle)
{
    _disableRunningTask();
    _setRPM(12);
    delay(20);
    _computeAngle();
    float origAngle = _angle;
    float minPulseWidth = 1500;
    float maxPulseWidth = 0;

    while (origAngle + 720 > _angle)
    {
        if (show_origAngle) {
            Serial.println(origAngle);
        }
        delay(1);
        _computeAngle();

        if (_pwmValue < minPulseWidth && _pwmValue >= 0)
            minPulseWidth = _pwmValue;
        if (_pwmValue > maxPulseWidth && _pwmValue < 1500)
            maxPulseWidth = _pwmValue;
    }
    rotateTo(origAngle);
    wait();
    adjustSignal(minPulseWidth, maxPulseWidth);
    Serial.print("Minimal PWM: ");
    Serial.print(_minPulseWidth);
    Serial.print(", maximal PWM: ");
    Serial.println(_maxPulseWidth);
}
/**
 * @brief Spin clockwise or anticlockwise at a the default RPM if the parameter unset.
 * Setting the parameter won't change the saved RPM of the servo. Instead use the method servo.setSpeed(); . 
 * Value must be between -140 and 140.
 * @param rpm 
 */
void ESP32Servo360::spin(float rpm)
{
    _disableRunningTask();
    _setRPM(rpm);
}
/**
 * @brief Spin clockwise or anticlockwise at a the default RPM if the parameter unset.
 * Setting the parameter won't change the saved RPM of the servo. Instead use the method servo.setSpeed(); . 
 * Value must be between -140 and 140.
 */
void ESP32Servo360::spin(void)
{
    _disableRunningTask();
    _setRPM(_rpm);
}
/**
 * @brief Wait for the motor to finish its rotation. Will hold the execution of the main loop().
 * 
 */
void ESP32Servo360::wait()
{
    while (_updateHandle != NULL) {
        delay(1);
    } 
}
/**
 * @brief true if servo is still executing a rotation or holding an angle.
 * Returns bool.
 * 
 * @return true 
 * @return false 
 */
bool ESP32Servo360::busy()
{
    return _updateHandle = NULL;
}
/**
 * @brief Change the maximum RPM (Rotation Per Minute) of your servo. Default is 70 RPM.
 * 
 * @param maxRpm 
 */
void ESP32Servo360::setSpeed(float maxRpm)
{
    _rpm = constrain(maxRpm, -MAX_RPM, MAX_RPM);
    _minRpm = constrain(_minRpm, 0, _rpm);
}
/**
 * @brief For easing rotations only, add more force when speed is minimal (at start & end of movement).
 * Default is 5.
 * 
 * @param minRpm 
 */
void ESP32Servo360::setAdditionalTorque(float minRpm) {
    _minRpm = constrain(abs(minRpm), 0, _rpm);
}
/**
 * @brief Minimal force required for the servo to move.
 * Minimal force may barely move the servo, bigger force may do infinite bounces.
 * Default is 7.
 * 
 * @param minTorque 
 */
void ESP32Servo360::setMinimalForce(float minTorque)
{
    _minTorque = max(abs(minTorque), (float)0);
}
/**
 * @brief Rotate from current position.
 * 
 * @param angle 
 */
void ESP32Servo360::rotate(float angle)
{
    _target += angle;
    _beginLoop();
}
/**
 * @brief Rotate to a specific position.
 * 
 * @param target 
 */
void ESP32Servo360::rotateTo(float target)
{
    _target = target + _offsetAngle;
    _beginLoop();
}
/**
 * @brief Rotate to a given angle with a ease-in-out-quad move.
 * 
 * @param target 
 */
void ESP32Servo360::easeRotateTo(float target)
{
    _target = target + _offsetAngle;
    _beginEase();
}
/**
 * @brief Rotate by given angle with a ease-in-out-quad move.
 * 
 * @param angle 
 */
void ESP32Servo360::easeRotate(float angle)
{
    _target += angle;
    _beginEase();
}
/**
 * @brief Disconnect the servo from its pins.
 * 
 * @return true 
 * @return false 
 */
bool ESP32Servo360::detach()
{
    if (!this->attached())
    {
        return false;
    }

    if (_channel == (channel_next_free - 1))
        channel_next_free--;

    ledcDetachPin(_ctrlPin);
    detachInterrupt(_feedbackPin);
    _resetFields();
    return true;
}
/**
 * @brief Get the amount of rotation. Angle will go below 0 and above 360 degrees.
 * Returns a float.
 * 
 * @return float 
 */
float ESP32Servo360::getAngle()
{
    _computeAngle();
    return _angle - _offsetAngle;
}
float ESP32Servo360::getSpeed()
{
    return ( _delta / 360 ) / ( _delta_time / 1e6F / 60 ) ; // ( degree_delta / 360 = rotation ) / ( delta_time in us / 1e6 / 60 = minute ) = rpm ; // 
}
/**
 * @brief Get number of turns. This will be reset after a reboot of the board.
 * Returns int.
 * 
 * @return int 
 */
int ESP32Servo360::getTurns()
{
    float angle = getAngle();
    return (angle - fmod(angle, 360)) / 360;
}
/**
 * @brief Get the orientation between 0 to 360 degrees.
 * Returns a float.
 * 
 * @return float 
 */
float ESP32Servo360::getOrientation()
{
    float angle = getAngle();
    return fmod(angle, 360);
}
/**
 * @brief Reset the number of turns.
 * 
 */
void ESP32Servo360::clearTurns()
{
    _angle = _orientation;
}
/**
 * @brief Returns the angle speed in rpm (integer) set by setSpeed();.
 * 
 * @return int 
 */
int ESP32Servo360::getStatedSpeed()
{
    return _rpm;
}
/**
 * @brief Stop and hold the servo to its current angle. Will be harder to turn the motor by hand. Disable this state by executing another rotation or calling servo.release();.
 * 
 */
void ESP32Servo360::hold()
{

    if (!_hold)
    {
        _disableRunningTask();
        _hold = true;
        delay(100);
        _computeAngle();
        _target = _angle;
        _beginHold();
    }
}
/**
 * @brief Releases the servo from its hold state.
 * 
 */
void ESP32Servo360::release()
{
    _hold = false;
    stop();
}
/**
 * @brief true if servo is attached to pins. Returns bool.
 * 
 * @return true 
 * @return false 
 */
bool ESP32Servo360::attached() const { return _ctrlPin != PIN_NOT_ATTACHED; }
/**
 * @brief Stop the rotation of the servo.
 * 
 */
void ESP32Servo360::stop()
{
    _disableRunningTask();
}

// The following ones are the internal functions.
/**
 * @brief Computing the actual angle: _orientation, actualizing the total _angle value.
 * 
 */
void ESP32Servo360::_computeAngle()
{
    float newOrientation = _fmap(_pwmValue, _minPulseWidth, _maxPulseWidth, 0, 360);
    _delta = newOrientation - _orientation;

    if (_delta > 180)
    {
        _delta -= 360;
    }
    else if (_delta < -180)
    {
        _delta += 360;
    }

    _angle += _delta;
    _orientation = newOrientation;
    _timeStamp1 = _timeStamp2;
    _timeStamp2 = esp_timer_get_time();
    _delta_time = (float) (_timeStamp2 - _timeStamp1);
}

void ESP32Servo360::_computeTarget()
{
    float theta = _target - _angle;
    _speed = constrain(_fmap(abs(theta), 0, _deceleration, _minTorque, MAX_RPM), 0, _rpm) * _sgn(theta);
    _setRPM(_speed);
}

inline void ESP32Servo360::_disableRunningTask()
{
    if (_updateHandle != NULL)
    {
        vTaskDelete(_updateHandle);
        _updateHandle = NULL;
    }

    _setRPM(0);
}

void ESP32Servo360::_setRPM(float rpm)
{

    //https://www.parallax.com/sites/default/files/downloads/900-00360-Feedback-360-HS-Servo-v1.2.pdf

    float tControl = 1500; //Default 0 RPM
    rpm = constrain(rpm, -MAX_RPM, MAX_RPM);

    if (rpm < 0)
    {
        tControl = _fmap(rpm, -MAX_RPM, 0, 1720, 1520);
    }
    else if (rpm > 0)
    {
        tControl = _fmap(rpm, 0, MAX_RPM, 1480, 1280);
    }

    float duty = tControl / (20000 /*20ms*/ / 65536.0 /*2 power of 16(bit)*/);

    ledcWrite(_channel, duty);
}

void ESP32Servo360::_resetFields(void)
{
    _feedbackPin = PIN_NOT_ATTACHED;
    _ctrlPin = PIN_NOT_ATTACHED;
    _channel = CHANNEL_NOT_ATTACHED;
    _offsetAngle = ZERO_ANGLE;
    _minPulseWidth = MIN_PULSE_WIDTH;
    _maxPulseWidth = MAX_PULSE_WIDTH;
}

void ESP32Servo360::_beginLoop()
{
    _disableRunningTask();
    _hold = false;
    xTaskCreate(
        _updateLoop,
        "update",
        1000,
        this,
        1,
        &_updateHandle // Task handle
    );
}

void ESP32Servo360::_beginEase()
{
    _disableRunningTask();
    _hold = false;
    xTaskCreate(
        _updateEase,
        "ease",
        1000,
        this,
        1,
        &_updateHandle // Task handle
    );
}

void ESP32Servo360::_beginHold()
{
    _disableRunningTask();

    xTaskCreate(
        _updateHold,
        "hold",
        1000,
        this,
        1,
        &_updateHandle // Task handle
    );
}

void ESP32Servo360::_isr(void *arg)
{

    ESP32Servo360 *s = (ESP32Servo360 *)arg;

    if (digitalRead(s->_feedbackPin))
    { // FALLING
        s->_prevTime = esp_timer_get_time();
    }
    else
    {
        s->_pwmValue = esp_timer_get_time() - s->_prevTime;
    }
}

void ESP32Servo360::_updateEase(void *arg)
{
    ESP32Servo360 *s = static_cast<ESP32Servo360 *>(arg);

    s->_computeAngle();
    float origAngle = s->_angle; // Get complete angle.
    float currAngle = origAngle;
    float peak = 0.5;
    float t = 0;
    float minForce = min(s->_minTorque + s->_minRpm, (float)s->_rpm);

    int direction = _sgn(s->_target - origAngle);

    while (t < 1.0)
    {
        Serial.println(t);
        s->_computeAngle();
        float currAngle = s->_angle;
        t = _fmap(currAngle, origAngle, s->_target, 0, 1);

        float e = (easeInOutQuad(t) - peak);
        float speed = _fmap(abs(e), 0, peak, s->_rpm, minForce);
        s->_setRPM(speed * direction);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);

    s->_setRPM(0);
    s->_updateHandle = NULL;
    vTaskDelete(NULL);
}

void ESP32Servo360::_updateLoop(void *arg)
{
    ESP32Servo360 *s = static_cast<ESP32Servo360 *>(arg);

    byte len = 32;
    int samples[len];
    int n = 100;
    byte slot = 0;
    int total = n * len;

    for (byte i = 0; i < len; i++)
    {
        samples[i] = n;
    }

    for (;;)
    { // infinite loop

        s->_computeAngle();
        s->_computeTarget();
        int speed = round(s->_speed);

        total = total - samples[slot] + speed;
        samples[slot] = speed;
        slot = (slot + 1) % len;

        if (abs((float)total / len - speed) < 1 && abs(speed) < 6)
        {
            s->_updateHandle = NULL;
            vTaskDelete(NULL);
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void ESP32Servo360::_updateHold(void *arg)
{
    ESP32Servo360 *s = static_cast<ESP32Servo360 *>(arg);
    for (;;)
    { // infinite loop
        s->_computeAngle();
        s->_computeTarget();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

float ESP32Servo360::easeInOutQuad(float t)
{
    return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

float ESP32Servo360::_fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int8_t ESP32Servo360::_sgn(int val)
{
    if (val < 0)
        return -1;
    if (val == 0)
        return 0;
    return 1;
}
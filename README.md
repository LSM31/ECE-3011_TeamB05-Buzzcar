# BuzzCar Firmware

Firmware for the ECE 3011 BuzzCar project using an **ESP32-C6-DevKitM-1**, **PlatformIO**, and **ESP-IDF**.

## Setup

1. Install VS Code.
2. Install the PlatformIO IDE extension.
3. Clone this repository.
4. Open the repository root in VS Code.

## Build

```powershell
pio run
```

## Upload

Find the ESP32 serial port:

```powershell
pio device list
```

Then upload:

```powershell
pio run -t upload --upload-port COM8
```

Replace `COM8` with the correct port.

## Serial Monitor

```powershell
pio device monitor -p COM8 -b 115200
```

## Project Structure

```text
src/
├── main.c
├── drv.c
├── drv.h
├── pins.h
└── CMakeLists.txt
```

- `main.c` — main BuzzCar behavior
- `pins.h` — GPIO assignments
- `drv.h` — public DRV8833 functions
- `drv.c` — DRV8833 GPIO and PWM implementation

## DRV8833 Motor Control

Initialize and wake the driver before commanding the motors:

```c
drv_init();
drv_wake();
```

Motor control:

```c
drv_set_motor_a(direction, speed);
drv_set_motor_b(direction, speed);
```

### Direction

```text
 1 = forward
 0 = stop
-1 = reverse
```

### Speed

Speed is an 8-bit PWM duty value:

```text
0   = 0%
128 ≈ 50%
255 = 100%
```

PWM is generated using the ESP32-C6 LEDC peripheral at **20 kHz** with **8-bit resolution**.

Examples:

```c
drv_set_motor_a(1, 255);   // full forward
drv_set_motor_a(-1, 128);  // ~50% reverse
drv_set_motor_a(0, 0);     // stop
```

The current implementation uses:

```text
Forward: IN1 = PWM, IN2 = 0
Reverse: IN1 = 0,   IN2 = PWM
Stop:    IN1 = 0,   IN2 = 0
```

## Testing Note

Initial testing showed one motor required approximately `195/255` duty to reliably start from rest. This may vary with motor, direction, load, battery voltage, and drivetrain friction.

## Git Workflow

Do development on a feature branch rather than directly on `main`.

```powershell
git switch -c <branch-name>
git add .
git commit -m "Describe changes"
git push -u origin <branch-name>
```
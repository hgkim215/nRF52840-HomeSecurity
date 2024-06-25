# Home Security
2024-1 마이크로프로세서 응용 수업 기말 프로젝트

## 팀 11조
- 21900215 김현기
- 21900420 안명훈

## 사용 방법
1. **NFC 태깅으로 문 열기**  
  NFC를 핸드폰으로 태깅해서 방의 문을 열 수 있습니다. 방문을 열어서 방 안에 출입할 수 있습니다. NFC를 태깅하게 되면 방문이 열림과 동시에, 부저에서 환영 음성을 재생하게 됩니다. 만약 외부인이 출입하는 경우에는 이 환영 메시지를 통해서 외부인이 출입했구나를 인지할 수 있습니다.
2. **Bluetooth 연결을 통해서 무선 통신**  
Bluetooth 연결을 nRF ToolBox 앱을 통해서 하고, 무선 명령어를 핸드폰에서 전송하는 행위를 통해서 보드를 동작시킬 수 있습니다. 명령어에는 pir, open, warning 등이 있습니다.
3. **PIR 센서를 이용한 Home 움직임 감지**  
보드의 PIR 센서를 사용해서 Home 내의 움직임 또한 감지할 수 있습니다. 움직임이 감지되었을 경우에 보드의 LED4가 꺼지고, 움직임이 감지가 되지 않으면 LED4가 켜지게 됩니다. 또한, Bluetooth를 통해서 메시지를 보냄으로써 PIR 센서의 상태를 바로 확인할 수 있습니다.
4. **BUZZER를 통해 소리로 경고 메시지와 환영 메시지**  
	Buzzer에 총 2가지 버전의 음성이 탑재되어있습니다.
1번째는 환영 음성과 2번째는 경고 음성입니다. 사용자가 NFC 태깅으로 들어올 때와 PIR에 사람 움직임 감지가 안될 때는 환영 음성을 들려주고, PIR에 사람이 감지되었을 때와 Warning 메시지를 수신하면 경고 음성을 출력합니다.
5. **Button 토글을 이용해서 방 안의 불 켜기, 끄기**  
보드의 Button을 토글함으로써 방 안의 불을 키고, 끌 수 있습니다, 현재는 보드의 LED를 토글하고 있지만, 좀 더 확장되는 것을 생각해보았을 때, 방 안의 불과 연결하여 방 안의 불을 토글하는 것으로 생각했습니다.


## 프로젝트 구조
```
.
├── CMakeLists.txt
├── CMakePresets.json
├── Kconfig
├── Kconfig.sysbuild
├── app.overlay
├── buzzer.overlay
├── dts
│   └── bindings
│       └── pwm-servo.yaml
├── led.overlay
├── pir.overlay
├── prj.conf
├── servo.overlay
└── src
    ├── ble_uart
    │   ├── CMakeLists.txt
    │   ├── ble_uart.c
    │   ├── uart_async_adapter.c
    │   └── uart_async_adapter.h
    ├── btn
    │   ├── CMakeLists.txt
    │   ├── asm.c
    │   ├── btn.c
    │   └── btn.h
    ├── buzzer
    │   ├── CMakeLists.txt
    │   ├── buzzer.c
    │   └── buzzer.h
    ├── led
    │   ├── CMakeLists.txt
    │   ├── led.c
    │   └── led.h
    ├── main.c
    ├── nfc
    │   ├── CMakeLists.txt
    │   └── nfc.c
    ├── pir
    │   ├── CMakeLists.txt
    │   ├── pir.c
    │   └── pir.h
    └── servo
        ├── CMakeLists.txt
        ├── servo.c
        └── servo.h
```
이 프로젝트의 폴더 구조는 다음과 같이 구성되어 있습니다:

1. `CMakeLists.txt`: 프로젝트 빌드를 위한 CMake 구성 파일입니다.
2. `CMakePresets.json`: CMake 프리셋 구성 파일입니다.
3. `Kconfig`: Kconfig 설정 파일입니다. Kconfig는 Zephyr OS 프로젝트에서 사용되는 구성 시스템입니다.
4. `Kconfig.sysbuild`: Kconfig 시스템 빌드 구성 파일입니다.
5. `app.overlay`, `buzzer.overlay`, `led.overlay`, `pir.overlay`, `servo.overlay`: Device Tree Overlay 파일들입니다. 이 파일들은 하드웨어 구성을 정의합니다.
6. `dts/bindings/pwm-servo.yaml`: Device Tree Binding 파일입니다. 이 파일은 PWM 서보 드라이버를 정의합니다.
7. `prj.conf`: Zephyr OS 프로젝트 구성 파일입니다.
8. `src/` 폴더:
   - `ble_uart/`: BLE UART 관련 소스 코드가 들어있습니다.
   - `btn/`: 버튼 관련 소스 코드가 들어있습니다.
   - `buzzer/`: 부저 관련 소스 코드가 들어있습니다.
   - `led/`: LED 관련 소스 코드가 들어있습니다.
   - `main.c`: 프로젝트의 메인 소스 코드 파일입니다.
   - `nfc/`: NFC 관련 소스 코드가 들어있습니다.
   - `pir/`: PIR 센서 관련 소스 코드가 들어있습니다.
   - `servo/`: 서보 모터 관련 소스 코드가 들어있습니다.

이 프로젝트는 Zephyr OS 기반으로 작성되었으며, 다양한 하드웨어 구성 요소(버튼, LED, 부저, PIR 센서, 서보 모터 등)를 포함하고 있습니다. 각 하드웨어 구성 요소에 대한 소스 코드와 Device Tree Overlay 파일이 별도의 폴더에 구성되어 있습니다. 이를 통해 프로젝트의 구조와 코드 관리가 체계적으로 이루어지고 있습니다. 

# proyecto-pico/build.sh
#!/bin/bash
export PATH="$HOME/opt/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin:$PATH"
echo "importado gcc-arm"
export PICO_SDK_PATH="$HOME/Documentos/rp2040/pico-sdk/"
echo "Importado pico-sdk"
#export PICO_TFLMICRO_PATH"$HOME/Documentos/Ingenieria/scripts/rp2040/pico-tflmicro/"
#echo "importado pico-tflmicro"
echo "Entorno RPi Pico Configurado"

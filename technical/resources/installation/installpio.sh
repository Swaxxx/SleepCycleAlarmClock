#!/bin/sh

#!/bin/sh
export PATH=$HOME/.local/bin/:$PATH

mkdir -p /tmp/se/.platformio
ln -s /tmp/se/.platformio $HOME/.platformio
# enlever ce qui vous ne sert pas :
pio platform install ststm32
pio platform install nordicnrf51
pio platform install nordicnrf52
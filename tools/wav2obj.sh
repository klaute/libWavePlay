#!/bin/bash

#-r 22050 \
#-r 11025 \
#-r 8000 \

IN=$1
OUTSMALL=${IN/.wav/_small.wav}
OUTNOHEADER=${IN/.wav/_noheader.wav}

sox \
--show-progress \
$IN \
-t wav \
--endian little \
-c 1 \
-1 \
-r 8000 \
$OUTSMALL \
polyphase

echo "$OUTSMALL generated..."

# ab den 45ten Byte ausgeben (44 Byte des Wave-Header überspringen)
cat $OUTSMALL | tail -c+45 > $OUTNOHEADER
echo "$OUTNOHEADER generated..."


cp $OUTNOHEADER data.wav
xxd -i data.wav | sed -e "s/unsigned char/prog_char/ig" | sed -e "s/wav_data/data_wav/ig" > wavedata.c
rm data.wav

echo "wavedata.c generated..."

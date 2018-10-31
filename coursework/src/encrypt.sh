#!/bin/bash

# duplicate stdin
exec 3<&0

if [[ ! $1 ]]; then
    TEXTTOENCRYPT="The lazy dog jumps over the quick fox that is brown"
else
    TEXTTOENCRYPT="$1"
fi

ENCRYPTFILE="toEncrypt.txt"
ENCRPYTEDFILE="encrypted.txt.enc"


if [[ ! -f "$ENCRYPTFILE" ]]; then
    echo "$TEXTTOENCRYPT" >> "$ENCRYPTFILE"
fi

openssl enc -aes128 -base64 -in "$ENCRYPTFILE" -out "$ENCRPYTEDFILE" <&3

#openssl aes128 -d -a -in toEncrypt.txt.enc -out secrets.txt.new

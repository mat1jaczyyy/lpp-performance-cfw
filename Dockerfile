FROM alpine:3.14
RUN apk add --no-cache alpine-sdk
RUN git clone https://github.com/mat1jaczyyy/LP-Firmware-Utility/ /tmp/utils

WORKDIR /tmp/utils
RUN make tools

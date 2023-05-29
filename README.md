# pdtonegen
ESP based hardware to produce tone patterns

# Component Documentation
## ESP8266
Wemos D1 Mini

## AD9833 Function Generator
[Data sheet](docs/ad9833.pdf)
https://majicdesigns.github.io/MD_AD9833/index.html

## 74HC Demultiplexer
[Data sheet](docs/CD74HC4051-etcTI.pdf)
https://roboway.in/shop/74hc4051-8-channel-analog-multiplexer-demultiplexer-breakout-board-for-arduino/ 

## PAM8403 Stereo 2x3W Amplifier

# Wiring
## ESP8266
    5V -> rail+ 
    G -> rail-
    D1 -> Demux/S0
    D2 -> Demux/S1
    D3 -> Demux/S2
    D5 -> AD9833/SCLK
    D7 -> AD9833/SDATA
    D8 -> AD9833/FSYNC

## AD9833 tone generator 
    VCC <- rail+
    DGND <- rail-
    SDATA <- ESP/D7
    SCLK <- ESP/D5
    FSYNC <- ESP/D8
    AGND <- rail-
    OUT -> Demux/Z

## 74HC4051 Demux
    VEE n/c
    VCC <- rail+
    GND <- rail-
    Z <- AD9833/OUT
    S0 <- ESP/D1
    S1 <- ESP/D2
    S2 <- ESP/D3
    E – n/c
    Y0 -> Amp1/L
    Y1 -> Amp1/R
    Y2 -> Amp2/L
    Y3 -> Amp2/R
    Y4..Y7 - n/c 

## Amp 1 
    L <- Demux/Y0
    R <- Demux/Y1
    Power+ <- rail+
    Power- <- rail-

## Amp 2
    L <- Demux/Y2
    R <- Demux/Y3
    Power+ <- rail+
    Power- <- rail-

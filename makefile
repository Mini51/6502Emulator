name: build

on:
  push:
    branches:
      - main

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout repository
      uses: actions/checkout@v2

    - name: Install g++
      run: sudo apt-get install -y g++

    - name: Build Linux binary
      run: make

    - name: Upload Linux binary as release asset
      uses: actions/upload-artifact@v2
      with:
        name: mos6502-linux
        path: build/mos6502

  build-windows:
    runs-on: windows-latest

    steps:
    - name: Checkout repository
      uses: actions/checkout@v2

    - name: Build Windows binary
      run: make

    - name: Upload Windows binary as release asset
      uses: actions/upload-artifact@v2
      with:
        name: mos6502-windows
        path: build/mos6502.exe

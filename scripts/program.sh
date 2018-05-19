#!/usr/bin/env bash
openocd -f /usr/local/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/local/share/openocd/scripts/target/stm32l0.cfg -f /home/cube/Projects/SDSusi/scr/program.proc

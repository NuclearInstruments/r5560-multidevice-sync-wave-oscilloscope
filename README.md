# Synchronize multiple R5560 / R5560SE to for simultaneous sampling on multiple device 

Thisexample explains how to synchronize multiple R5560/R5560SE to extend the number of input channels sampled simultaneously.
This document covers the following task:
• How to distribute a common clock
• How to synchronize the acquisition between multiple DAQ and device
• How to distribute common trigger signal
To synchronize multiple device it is mandatory to have a DAQ and BASE operational system newer than 2022.7.0.1. Check on the display and on the web page of the DAQ and of the BASE of the R5560.

# Obtaining 16k samples per channel on 32 channels on R5560A
In the example we use the Oscilloscope Dual and we configured SciCompiler to use the board profile Z-7030-MINIMAL. In this configuration the optical link, the DMA and the horizontal link are disable in order to free all the available memory on the FPGA. This allows to extend the maximum memory depth from 8k to 16k per channel

# Documentation

Read documentation in Doc folder

# Compile FPGA
In order to compiler the scf file for R5560A, download and install SciCompiler 2022.7.0.4 or newer. http://www.scicompiler.cloud
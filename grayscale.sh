#!/bin/bash -eux

gs \
  -sOutputFile=articles/tbf05-yge-book-gray.pdf \
  -sDEVICE=pdfwrite \
  -dPDFX \
  -dCompatibilityLevel=1.3 \
  -sColorConversionStrategy=Gray \
  -dProcessColorModel=/DeviceGray \
  -dNOPAUSE \
  -dBATCH \
  articles/tbf05-yge-book.pdf

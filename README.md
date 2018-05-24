## Overview

Developed by BioTuring (www.bioturing.com), <i><b>mdup</b></i> is a tool that preprocess cloud-read data (read has barcode). <i><b>mdup</b></i> will do: 

- Markdup PCR duplication, remove not primary reads, duplicate reads, secondary alignment, unmapped reads.
- Detect molecule by clustering reads have same barcode into group.
- Get stats about sequencing and GEM performance.

## Install

```shell
git clone https://github.com/bioturing/mdup.git
cd mdup
sh build.sh
```

## Usage

<i><b>mdup</b></i> take a BAM file as input, the Bam file must be sorted by coordinate and be indexed. Recommend using BWA to align cloud-read to referenece. All alignment record must have BX:Z: tag present for barcode.

<i><b>mdup</b></i> will generate some file in output directory:

- output.bam : new BAM file after remove unneeded reads.
- molecule.tsv : all molecule detected info.
- summary.inf : stats about sequencing and GEM performance.
- plot.html : plot of some metrics of stats.

```
./mdup [option] in.bam

Optional arguments:
  -t INT                number of threads [default: 1]
  -o DIR                output directory [default: "./mdup_out/"]
  -g FILE               reference file that generated bam file (for better stats)
  -k                    keep all record from BAM file, turn on duplicate bit flag instead.
```

# Contacts

Please report any issues directly to the github issue tracker.

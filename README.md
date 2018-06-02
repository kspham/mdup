## Overview

Developed by BioTuring (www.bioturing.com), <b>mdup</b> is a tool that preprocess cloud-read data (read has barcode). <b>mdup</b> will do:

- Remove duplicate reads, remove not primary reads, secondary alignment, unmapped reads.
- Detect molecule by clustering reads have same barcode into group.
- Get stats about sequencing and GEM performance.

Two reads are consider duplicate if they share same mapped position, mapped target, cigar,
mate info (if paired-end).

## Install

```shell
git clone https://github.com/kspham/mdup.git
cd mdup
bash build.sh
```

## Usage

<b>mdup</b> take a BAM file as input, the Bam file must be sorted by coordinate and be indexed. Recommend using BWA to align cloud-read to referenece. All alignment record must have BX:Z: tag present for barcode.

<b>mdup</b> will generate some file in output directory:

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
  -k                    don't mark duplicate.
```

# Contacts

Please report any issues directly to the github issue tracker.

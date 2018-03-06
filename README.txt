----------------------------------------------------------------
BUILDING

First, you need the boost C++ library. 
In particular, on a debian-based system, the packages:
	libboost-program-options-dev

Then just type:
  make

You may need to edit the makefile 

uncomment path_boost_lib and precise the boost library path
 

----------------------------------------------------------------

DETAILS


The following options are supported, with defaults given by (=default):
Usage: options_description [options]
Allowed options:
  -h [ --help ]                      produce help message
  -i [ --input-file ] arg            input files list
  -o [ --output-file ] arg (=stdout) output file
  -e [ --evalue ] arg (=10)          evalue
  -p [ --pident ] arg (=0)           pident
  -E [ --evalueStrict ] arg (=10)    Strict evalue
  -P [ --pidentStrict ] arg (=0)     Strict pident


----------------------------------------------------------------

INPUT
MosaicFinder takes 
  -a blast output tabular file (option -m8).
or 
  -a tabular file with an header containing at least:
    - qseqid means Query Seq-id
    - sseqid means Subject Seq-id
    - evalue means Expect value
    - pident means Percentage of identical matches 
    and optionally to calculate the fusion point: 
    - qstart means Start of alignment in query
    - qend means End of alignment in query
    - sstart means Start of alignment in subject
    - send means End of alignment in subject.

----------------------------------------------------------------

OUTPUT
MosaicFinder outputs a table of genes and gene families involved in fusion events. This table indicates the fusion event which genes
are involved in, and their groupings into component families. It additionally indicates a fusion point if the alignment is provided.
The columns are defined as follows:

-gene_id means query id
-fusion_event means the number of the fusion events of the query sequence
-family_number means the number of the component family the sequence is a part of (0 for sequence corresponding to a fused gene)
-start_med_align1 means median of the start of alignment for sequence of component family 1
-end_med_align1 means median of the end of alignment for sequence of component family 1
-start_med_align2 means median of the start of alignment for sequence of component family 2
-end_med_align2 means median of the end of alignment for sequence of component family 2
-break_point means fusion point corresponding to the middle point between the median alignments of each component families.






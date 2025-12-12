---
title: Start using Gynx
subject: Gynx Quickstart
short_title: Get Started
description: Using Gynx in a nutshell.
kernelspec:
  name: xcpp17
  display_name: C++17
---

# Start using Gynx

---

```{code-cell} cpp
#pragma cling add_include_path("/home/armin/src/sq3p/include")
#pragma cling add_include_path("/home/armin/src/g3p/include")

#include <fstream>

#include <g3p/gnuplot>
#include <sq3p/sq.hpp>
#include <sq3p/io/faqz.hpp>
```

Making a biological sequence in Gynx is easy:

```{code-cell} cpp
sq3p::sq s{"ACGT"};
```
Or even easier like this:
```{code-cell} cpp
auto t = "ACGT"_sq;
(s == t)
```
It's also easy to load from compressed/uncompressed fasta/fastq files. First let's download a sample file:
```{code-cell} cpp
!wget https://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/204/255/GCF_000204255.1_ASM20425v1/GCF_000204255.1_ASM20425v1_genomic.fna.gz
```
+++
```{code-cell} cpp
sq3p::sq plasmid;
plasmid.load("GCF_000204255.1_ASM20425v1_genomic.fna.gz", "NC_017288.1", sq3p::in::faqz());
(7553 == std::size(plasmid))
```
+++
```{code-cell} cpp
sq3p::sq plot = plasmid(0, 500);
// g3p::gnuplot gp("test.log");
g3p::gnuplot gp;

gp
( "set pm3d at b" )
( "set view map" )
( "set palette gray negative" )
( "set size square" )
// ( "unset xtics" )
// ( "unset ytics" )
// ( "unset colorbox" )
;
gp("splot '-' using 1:2:3 with pm3d");
for (size_t i = 0; i < std::size(plot); i += 10)
{
    for (size_t j = 0; j < std::size(plot); j += 10)
        gp << i << j << (plot[i] == plot[j] ? 1 : 0) << "\n";
    gp << "\n";
}
gp << g3p::end << g3p::endl
```
+++
```{code-cell} cpp
gp  ( "set cbrange [0:10]" )
    ( "splot '-' using 1:2:3 with pm3d" )
;
for (size_t i = 0; i < std::size(plot); ++i)
{
    for (size_t j = 0; j < std::size(plot); ++j)
        if (plot[i] == plot[j])
            gp << i << j << 10 << "\n";
    gp << "\n";
}
gp << g3p::end << g3p::endl
```
+++
```{code-cell} cpp
auto plot = "AGCCAACCACCCCCG"_sq;
auto gynx_x = "CCAATATTATC"_sq;
auto gynx_y = "CCTATGTTACT"_sq;

gp
( "set size square" )
( "unset key")
( "set xrange [0:14]" )
( "set yrange [0:14]" )
;
// gp("plot '-' u 1:2 w dot lc \"black\"");
gp("plot '-' u 1:2:3:xticlabels(4):yticlabels(5) w circles lc \"black\" fill solid noborder");
for (size_t i = 0; i < std::size(plot); ++i)
{
    for (size_t j = 0; j < std::size(plot); ++j)
        if (plot[i] == plot[j])
            gp("%d %d %f %c %c", i, j, 0.5, plot[i], plot[j]);
    gp << "\n";
}
gp << g3p::end << g3p::endl;
gp
```
+++
```{code-cell} cpp
plasmid["int_test"] = 19;
plasmid["float_test"] = 3.14f;
// plasmid["wrong"] = std::vector<int>{1, 2, 3, 4};
std::cout << plasmid;
```
+++
```{code-cell} cpp
sq3p::sq s{"ACGT"};
s["test"] = 33;
// std::cout << s;
s
```
+++
```{code-cell} cpp
std::stringstream ss;
ss << s;
sq3p::sq t;
ss >> t;
(s == t)
```
+++
```{code-cell} cpp
std::cout << t;
```
+++
```{code-cell} cpp
std::stringstream ss;
ss << plasmid;
sq3p::sq p;
ss >> p;
(p == plasmid)
```
+++
```{code-cell} cpp
sq3p::sq s{"ACGT"};
s["test"] = 33;
s["test2"] = 1.33f; 
std::ofstream os("test.sq3p");
os << s;
os.close();
```
+++
```{code-cell} cpp
sq3p::sq s{"ACGT"};
s["test"] = 33;
s["test-void"] = {};
s["test-bool"] = true;
s["test-float"] = 3.14f;
s["test-vector-int"] = std::vector<int>{ 1, 2, 3, 4 };
std::cout << s;
```
+++
```{code-cell} cpp
std::stringstream ss;
ss << s;
sq3p::sq t;
ss >> t;
std::cout << t;
```

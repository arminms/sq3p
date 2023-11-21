## fetching sample genome (Chlamydia psittaci 6BC)
#
FetchContent_Declare(
  sample_genome
  URL      https://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/204/255/GCF_000204255.1_ASM20425v1/GCF_000204255.1_ASM20425v1_genomic.fna.gz
  URL_HASH MD5=599418042b774c4e200406d290f4a863
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_Populate(sample_genome)

## add path for unit tests
#
add_definitions(-DSAMPLE_GENOME="${sample_genome_SOURCE_DIR}/GCF_000204255.1_ASM20425v1_genomic.fna.gz")

## defining target for unit tests
#
add_executable(unit_tests unit_tests.cpp)

## defining link libraries for unit tests
#
target_link_libraries(unit_tests PRIVATE
  ${PROJECT_NAME}::${PROJECT_NAME}
  Catch2::Catch2WithMain
  ZLIB::ZLIB
)

## finally adding unit tests
#
catch_discover_tests(unit_tests)

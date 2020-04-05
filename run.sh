verb='silent'
seed='12345678'
loops='500000000'
tasks='600'

echo "time ./OpenCLSumRnd $verb OpenCL $seed $loops $tasks"
time ./OpenCLSumRnd $verb OpenCL $seed $loops $tasks

tasks='12'
echo "time ./OpenCLSumRnd $verb OpenMP $seed $loops $tasks"
time ./OpenCLSumRnd $verb OpenMP $seed $loops $tasks

#echo "time ./OpenCLSumRnd $verb OneThread $seed $loops $tasks"
#time ./OpenCLSumRnd $verb OneThread $seed $loops $tasks



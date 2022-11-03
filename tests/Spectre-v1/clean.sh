cd PHT/
for d in *; do
	cd $d;
	rm -r klee-*/
	rm klee-last
	cd ../
done

cd ../BTB/
for d in *; do
	cd $d;
	rm -r klee-*/
	rm klee-last
	cd ../
done


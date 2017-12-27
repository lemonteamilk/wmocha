FILE *cl;
char *filename = "commlog.clog";
int nsize;
int nrank;
int nroot = 0;
int i, j = 0;
int *catchbuff;//define catch buff
int catchsize, catchunits;
int throwsize, throwunits;
double ts, te, t;
double *tseries = NULL;
MPI_Datatype MPI_FPREC = (sizeof(fprec) == sizeof(double) ? MPI_DOUBLE : MPI_FLOAT);

cl = fopen(filename, "ab+");

MPI_Barrier(MPI_COMM_WORLD);
ts = MPI_Wtime();

MPI_Comm_size(MPI_COMM_WORLD, &nsize);
MPI_Comm_rank(MPI_COMM_WORLD, &nrank);
/*Referenced from http ://mpitutorial.com/tutorials/mpi-scatter-gather-and-allgather/----------------------------*/
int throrg[] = { 108, 101, 109, 111, 110, 116, 101, 97, 45, 45, 45, 97 };//define throw message
throwsize = sizeof(throrg);
throwunits = throwsize / sizeof(int);
fprintf(cl, "\n===data at the end is %c\n", throrg[throwunits-1]);
throrg[throwunits - 1] = throrg[throwunits - 1] + nrank;//make difference for each rank
fprintf(cl, "\nThrow message is ");//verify the throwing message
for (i = 0; i < throwunits; i++) {
  fprintf(cl, "%c", throrg[i]);
}
fprintf(cl, "\n");
if (nrank == nroot) {//allocate memory at base rank
  catchbuff = (int *)malloc(nsize * throwunits * sizeof(int));//allocate catch buff in 1d array
  tseries = malloc(sizeof(double)*(nsize + 1));
  fprintf(cl, "+++Throw message size is %d, total %d units in the message. Timeseries length %zd.\n", throwsize, throwunits, sizeof(tseries));
}
te = MPI_Wtime();
t = te - ts;
MPI_Gather(throrg, throwunits, MPI_INT, catchbuff, throwunits, MPI_INT, nroot, MPI_COMM_WORLD);
MPI_Gather(&t, 1, MPI_DOUBLE, tseries, 1, MPI_DOUBLE, nroot, MPI_COMM_WORLD);
/*Reference END----------------------------------------------------------------------------------------------------*/
if (nrank == nroot) {//print gatherd data at base rank
  catchsize = sizeof(&catchbuff);
  catchunits = nsize * throwunits;
  fprintf(cl, "+++Catchbuff size is %d, catchbuff can hold %d units.\n", catchsize, catchunits);
//print out caught data
  fprintf(cl, "|++|PROCTIME:|%f|---|DATACATCH:|", t);
  for (i = 0; i < catchunits; i++) {
    if ((i % throwunits) != (throwunits-1)) {
      fprintf(cl, "%c", catchbuff[i]);
    }
    else if (i == catchunits - 1) {
      fprintf(cl, "|Data from processor PROCESSOR|%c|++|\n", catchbuff[i]);
    }
    else {
      fprintf(cl, "|Data from processor PROCESSOR|%c|++|\n|++|PROCTIME:|%f|---|DATACATCH:|", catchbuff[i], t);
    }
  }
}
fclose(cl);

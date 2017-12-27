//MPI applicate part
MPI_Datatype MPI_FPREC = (sizeof(fprec) == sizeof(double) ? MPI_DOUBLE : MPI_FLOAT);
FILE *dt;
char *filename = "testoutput.dat";
char nstr[1000];
int nsize;
int nrank;
int nroot = 0;
int i,j = 0;
double ts, te, t;//define variables to evaluate process time
double *tseries = NULL;//address to store time

dt = fopen(filename, "a+");

MPI_Barrier(MPI_COMM_WORLD);
ts = MPI_Wtime();

MPI_Comm_size(MPI_COMM_WORLD, &nsize);
MPI_Comm_rank(MPI_COMM_WORLD, &nrank);

int tflag_local[16];//toal 16 cores
int *tflag_catched = NULL;
int rthrow = 0;//send data
int wtoggle = 0;//if data caught, toggle on
for (i = 1; i < nsize + 1; i++) { tflag_local[i] = 0; }//initialize rthrow
tflag_local[nrank+1] = 1;
/*Referenced from http ://mpitutorial.com/tutorials/mpi-scatter-gather-and-allgather/----------------------------*/
if (nrank == 0) {
  tflag_catched = malloc(sizeof(int) * (nsize+1));
  tseries = malloc(sizeof(double)*(nsize + 1));
  for (j = 1; j < nsize + 1; j++) {
    tflag_catched[j] = 0;
    tseries[j] = 0;
    wtoggle = 65;//toggled A
  }
}
else {
  wtoggle = 89;//toggle Y
  fprintf(dt, "##This processor is not at the root rank.(RANK%d,Toggled%d).\n", nrank, wtoggle);
}
rthrow = wtoggle;
if (rthrow == 0 && nrank != 0) {
  wtoggle = 78;//toggle N
  fprintf(dt, "++This processor is not activated.(RANK%d,Toggled%d).\n", nrank, wtoggle);
}
te = MPI_Wtime();
t = te - ts;
MPI_Gather(&rthrow, 1, MPI_INT, tflag_catched, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Gather(&t, 1, MPI_DOUBLE, tseries, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
/*Reference END----------------------------------------------------------------------------------------------------*/
//MPI_Reduce(&tflag_local, &prl_flag, 1, MPI_INT, MPI_SUM, nroot, MPI_COMM_WORLD);
if (nrank == 0) {
  sprintf(nstr, "---------------------------------------\n|PRANK:|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|\n|GATHR:|%c|%c|%c|%c|%c|%c|%c|%c|%c|%c|%c|%c|%c|%c|%c|%c|\n", tflag_catched[1], tflag_catched[2], tflag_catched[3], tflag_catched[4], tflag_catched[5], tflag_catched[6], tflag_catched[7], tflag_catched[8], tflag_catched[9], tflag_catched[10], tflag_catched[11], tflag_catched[12], tflag_catched[13], tflag_catched[14], tflag_catched[15], tflag_catched[0]);
  fprintf(dt, nstr);
  sprintf(nstr, "|LOCAL:|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|\n", tflag_local[1], tflag_local[2], tflag_local[3], tflag_local[4], tflag_local[5], tflag_local[6], tflag_local[7], tflag_local[8], tflag_local[9], tflag_local[10], tflag_local[11], tflag_local[12], tflag_local[13], tflag_local[14], tflag_local[15], tflag_local[16]);
  fprintf(dt, nstr);
  sprintf(nstr, "|RANK0 GATHERED DATA<------|\n---------------------------------------\n");
  fprintf(dt, nstr);
}
fclose(dt);

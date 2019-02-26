#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
//#include <tbb/tbb.h>

using namespace std;


//Position
void update_pos(vector<double>& x,vector<double>& y,vector<double>& z, vector<double>& vx,vector<double>& vy,vector<double>& vz,vector<double>& accx,vector<double>& accy, vector<double>& accz){
	
	for(int i= 0; i < x.size(); l++){
		x[i]=x[i] + vx[i] * (dt) + 0.5 * accx[i] * (dt)*(dt);
		y[i]=y[i] + vy[i] * (dt) + 0.5 * accy[i] * (dt)*(dt);
		z[i]=z[i] + vz[i] * (dt) + 0.5 * accz[i] * (dt)*(dt); 
	}
	
};

//Acceleration
void get_acc(vector<double>& x,vector<double>& y,vector<double>& z, vector<double>& accx,vector<double>& accy, vector<double>& accz){
	
        vector<double> force_x(x.size());
		vector<double> force_y(y.size());
		vector<double> force_z(z.size());  
		

	for(int i= 0; i < x.size(); i++){
		//Classical mechanics F = mc | Average acceleration delta_v/delta_t 
		vector<double> force_x[i] = vm*accx[i];
		vector<double> force_y[i] = vm*accy[i];
		vector<double> force_z[i] = vm*accz[i];
		for(int j= 0; j < x.size(); j++){
			if(i==j)
				continue;
			        double dx,dy,dz,dist;
                    dx = x[i] - x[j];
                    dy = y[i] - y[j];
                    dz = z[i] - z[j];
                    dist = sqrt((dx*dx)+(dy*dy)+(dz*dz));
		}
		//a=F/m
		 accx[i] = force_x[i]/vm;
		 accy[i] = force_y[i]/vm;
		 accz[i] = force_z[i]/vm;
	}

};

//Velocity
void update_vel(vector<double>& vx,vector<double>& vy,vector<double>& vz,vector<double>& accx,vector<double>& accy, vector<double>& accz, vector<double>& prev_accx,vector<double>& prev_accy, vector<double>& prev_accz){
	
	for(int i= 0; i < vx.size(); l++){
		
		//avg_acc = ( prev_accel + new_acceleration )/2;
		double avg_accx,avg_accy,avg_accz;
		avg_accx = (prev_accx[i] + accx[i])/2;
		avg_accy = (prev_accy[i] + accy[i])/2;
		avg_accz = (prev_accz[i] + accz[i])/2;
		//vel += avg_acc * dt;
		vx[i] += avg_accx[i] * dt;
		vy[i] += avg_accy[i] * dt;
		vz[i] += avg_accz[i] * dt;
	}

};

  /**
   Velocity Verlet integration - The improved Euler.
   Original Verlet - Störmer's original method.
   */
   
void velocity_Verlet(vector<double> x, vector<double> y, vector<double> z, vector<double> vx, vector<double> vy, vector<double> vz, vector<double> vm, double dt,int time_steps, char *output_file) {
    //tbb::task_scheduler_init scheduler(1);
    vector<double> accx(x.size());
    vector<double> accy(y.size());
    vector<double> accz(z.size());
    vector<double> prev_accx(x.size());
    vector<double> prev_accy(y.size());
    vector<double> prev_accz(z.size());

    ofstream fout;
    fout.open(output_file);
    fout << "x, " << "y, " << "z, " << "vx, " << "vy, "<< "vz, "<< "vm " << endl;
    get_acc(x,y,z,accx,accy,accz);
	
	/* OPEN MPI */
    //#pragma omp parallel for 

    /* TBB */
   //tbb::parallel_for(tbb::blocked_range<int>(0, x.size()),
   //[&] (const tbb::blocked_range<int>& time_steps) {
		for (int i = time_steps.begin(); i < time_steps.end(); i++){
			//#pragma omp atomic ??
			update_pos(x,y,z,vx,vy,vz,accx,accy,accz);
			get_acc(x,y,z,accx,accy,accz);
			update_vel(vx,vy,vz,accx,accy,accz,prev_accx,prev_accy,prev_accz);
		}
	//}
	//);
	
	fout.close();
}

int main(int argc, char* argv[]) {
	char *initial_state_file_arg = argv[1];
	double dt = atof(argv[2]);
	double time_steps = atof(argv[3]);
	char *output_file_arg = argv[4];
	 
	ifstream initial_state_file(initial_state_file_arg);
	 
	if (!initial_state_file) {
		cout << "Can't open the initial_state_file\n";
	}

	int n;
	initial_state_file >> n;

	vector<double> x(n);
    vector<double> y(n);
    vector<double> z(n);
    vector<double> vx(n);
    vector<double> vy(n);
    vector<double> vz(n);
    vector<double> vm(n);
	
	for (int i = 0; i < n; i++ ) {
		initial_state_file >> x[i];
        initial_state_file >> y[i];
        initial_state_file >> z[i];
       	initial_state_file >> vx[i];
       	initial_state_file >> vy[i];
       	initial_state_file >> vz[i];
        initial_state_file >> vm[i];
	}
		
velocity_Verlet(x,y,z,vx,vy,vz,vm,dt,time_steps,output_file);

return 0;
}





#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>
#include"lcgrand.h"


struct node{

    double arrivaltime;
    double servicetime;
    double departime;
    struct node *l;

};

struct hd{

    int serv_used;
    double departime;

};

struct Node *creat(double arrivaltime , double servicetime){

    struct node *temp=(struct node *)malloc(sizeof(struct node));
    temp->arrivaltime= arrivaltime;
    temp->servicetime= servicetime;
    temp->departime= arrivaltime;
    temp->l=NULL;
    return temp;

}

double poisson_intertime(double rate){

    double interarrivaltime= -log(lcgrand())* (60.0/rate);
    return interarrivaltime;

}

double find_min_depart(struct hd *Serv, int S){

    double min= Serv[0].departime;
    int i;
    for(i=1;i<S;i++){

        if(min > Serv[i].departime){

            min=Serv[i].departime;

        }

    }
    return min;

}

struct node *getarrivalpacketinfo(double lambda, double mu, int n){

    double tempservicetime = poisson_intertime(mu);
    struct node *root = creat(0, tempservicetime);
    struct node *temp=root;
    int i;
    for(i=0;i<n-1;i++){

        double tempintertime = poisson_intertime(lambda);
        tempservicetime = poisson_intertime(mu);
        temp->l= creat(temp->arrivaltime + tempintertime, tempservicetime);
        temp=temp->l;

    }
    return root;
}

int num_Q=0;
struct Queue{

    struct node *l;

};

void arrive(struct hd Serv[], struct node *temp, int S , int K, double mu, struct Queue Q[]){

    int j;
    for(j=0;j<S;j++){

        if(Serv[j].serv_used == 0){
            
            temp->departime=temp->arrivaltime + temp->servicetime;
            Serv[j].departime = temp->departime;
            Serv[j].serv_used = 1;
            return;

        }

    }

    if(num_Q <= K-1){

        Q[num_Q].l=temp;
        num_Q++;

    }

    return;

}

void departure(struct hd Serv[], int S , int K, double mu, struct Queue Q[]){

    int i;
    for(i=0;i<S;i++){

        if(find_min_depart(Serv, S) == Serv[i].departime){

            break;

        }

    }
    if(num_Q == 0){

        Serv[i].serv_used = 0;
        Serv[i].departime = INT_MAX;

    }

    else{

        num_Q--;
        Q->l->departime = Serv[i].departime + Q->l->servicetime;
        Serv[i].departime = Q->l->departime;

        for(i=0;i<num_Q+1;i++){
            
            Q[i].l=Q[i+1].l;
            Q[i+1].l=NULL;

        }

    }

}

int factorial(int n){
    int i;
    if(n==1 || n==0){
        return 1;
    }
    for(i=n-1;i>0;i--){
        n=n*i;
    }
    return n;
}

double P0(float lambda, float mu, int s, int k){
	double sum = 1.0;
	int i;
	for(i=1;i<s;i++){
        sum += pow((lambda/mu),i) / factorial(i);
    }
	sum += (pow((lambda/mu), s) / factorial(s) * ((1-pow((lambda/(s*mu)), (k-s+1))) / (1-(lambda/(s*mu)))));
	return 1/sum;
}

double Pn(float lambda, float mu, int s, int k, int n){
	if(n<s){
        return ((pow((lambda/mu), n)) / factorial(n)) * P0(lambda, mu, s, k);
    }
		
	return ((pow((lambda/mu), n)) / (factorial(s) * pow(s, (n-s)))) * P0(lambda, mu, s, k);
}

double Lq_math(float lambda, float mu, int s, int k){
	double result = 0;
	int i;
	for(i=s;i<k+1;i++)
		result += (i-s)*Pn(lambda, mu, s, k, i);
	return result;
}

double L_math(float lambda, float mu, int s, int k){
	double sum1 = 0, sum2 = 0, temp;
	int i;
	for(i=0;i<s;i++){

		temp = Pn(lambda, mu, s, k, i);
		sum1 += i*temp;
		sum2 += temp; 

	}
	sum2 = s * (1- sum2);
	return (Lq_math(lambda, mu, s, k) + sum1 + sum2);
}



int main(){

    FILE *fp;
    fp = fopen("mmsk.csv", "a+"); 
    double W = 0.0, Wq =0.0, L = 0.0, Lq= 0.0;
	int S = 2, K = 8, n=1000000, i,j;
    double lambda=25.0;
    double mu=40.0;

        struct hd Serv[S];
        for(i=0;i<S;i++){

            Serv[i].serv_used=0;
            Serv[i].departime=INT_MAX; //get huge value
        
        }

        struct Queue Q[K];
        for(i=0;i<K;i++){

            Q[i].l=NULL;

        }
    
        struct node *root=getarrivalpacketinfo(lambda, mu, n);
        struct node *temp=root;

        while(temp != NULL){
        
            if(temp->arrivaltime < find_min_depart(Serv, S)){

                arrive(Serv, temp, S , K, mu, Q);
                temp=temp->l;

            }

            else{

                departure(Serv, S, K, mu, Q);

            }
        
        }

        temp=root;
        int count=0;
        double totalservicetime=0, sum=0.0;

        while(temp->l!= NULL){

            if(temp->departime != temp->arrivaltime){
                
                W+=temp->departime - temp->arrivaltime;
                totalservicetime+=temp->servicetime;
                count++;

            }
            temp=temp->l;

        }


        printf("%.8lf, ", W/(temp->departime)); //L
        printf("%.8lf, ", (W-totalservicetime)/(temp->departime)); //Lq
        printf("%.8lf, ", W/count/60); //W
        printf("%.8lf\n", (W-totalservicetime)/count/60); //Wq

        printf("%.8lf, ", L_math(lambda, mu, S, K)); //L
	    printf("%.8lf, ", Lq_math(lambda, mu, S, K)); //Lq
	    printf("%.8lf, ", L_math(lambda, mu, S, K) / (lambda*(1-Pn(lambda, mu, S, K, K)))); //W
	    printf("%.8lf ", Lq_math(lambda, mu, S, K) / (lambda*(1-Pn(lambda, mu, S, K, K)))); //Wq



        fprintf(fp,"%.8lf,", W/(temp->departime)); //L
        fprintf(fp,"%.8lf,", (W-totalservicetime)/(temp->departime));//Lq
        fprintf(fp,"%.8lf,", W/count/60);//W
        fprintf(fp,"%.8lf,,", (W-totalservicetime)/count/60);//Wq
    
        fprintf(fp,"%.8lf,", L_math(lambda, mu, S, K));//L
	    fprintf(fp,"%.8lf,", Lq_math(lambda, mu, S, K));//Lq
	    fprintf(fp,"%.8lf,", L_math(lambda, mu, S, K) / (lambda*(1-Pn(lambda, mu, S, K, K))));//W
	    fprintf(fp,"%.8lf\n", Lq_math(lambda, mu, S, K) / (lambda*(1-Pn(lambda, mu, S, K, K))));//Wq

    fclose(fp);
    return 0;
}
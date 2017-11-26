#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<fstream>
#include<vector>
#include<cmath>


double resultToDoubleNormalized(const char *a){
    if((*a) == 'p') return 1.0;
    if((*a) == 'e') return 0.0;
    else return 0.5;
}


double characterToDoubleNormalized(const char *a){
    if((*a) == '?') return 0.0;
    return ((int) (*a) - 97.0)/(26.0);
}

class StringArray{
    std::vector<std::string> strings;
    int count;
public:
    StringArray(std::string line, char delimiter){
        std::string temp;
        count = 0;
        for(char a:line){
            if(a == delimiter)
            {
                strings.push_back(temp);
                temp = "";
                count++;
            }else{
                temp += a;
            }
        }
        strings.push_back(temp);
        count++;

    }



    std::pair<std::vector<double>, double> getNormalizedData(){
        std::vector<double> inputParameters;
        inputParameters.reserve((this->count - 1) * sizeof(double));
        for(int i=1;i<count;i++){
            inputParameters.push_back(characterToDoubleNormalized(this->strings[i].c_str()));
        }

        return std::make_pair(inputParameters, resultToDoubleNormalized(this->strings[0].c_str()));
    }

    void display(){
        std::cout<<std::endl<<"-----------Printing-----------" <<this->count;
        for(int i=0;i<count;i++){
            std::cout<<this->strings[i]<<'\t';
        }
        std::cout<<std::endl<<"-----------End Printing-----------";
    }
    ~StringArray(){
    }
};


// Creates a random vector of specified size
template<typename T>
std::vector<T> getRandomVector(int size)
{
	std::vector<T> temp;

	for (int i = 0; i < size; i++)
	{
		temp.push_back( (T) rand() / RAND_MAX);
	}
	return temp;
}


class Perceptron {
	std::vector<std::vector<double>> weights;
	std::vector<double> thresholds;
	int num_inputs, num_neurons;
	double learning_rate = 0.1;
public:
	Perceptron(int num_inputs,int num_neurons, double learning_rate):num_inputs(num_inputs),num_neurons(num_neurons),learning_rate(learning_rate)
	{
		for (int i = 0; i < num_neurons; i++) {
			auto vector = getRandomVector<double>(num_inputs);
			weights.push_back(vector);
		}

		thresholds = getRandomVector<double>(num_neurons);

	}
	void train(std::vector<double> input, std::vector<double> target)
	{
		for (int i = 0; i < num_neurons; i++)
		{
			double h = 0;
			for (int j = 0; j < num_inputs; j++)
			{
				h += input[j] * weights[i][j];
			}
			double output = h > thresholds[i] ? 1.0 : 0.0;
			for (int j = 0; j < num_inputs; j++)
			{
				weights[i][j] += learning_rate * (target[i] - output) * input[j];
			}
 		}
	}

	std::vector<double> test(std::vector<double> input)
	{
		std::vector<double> temp;
		for (int i = 0; i < num_neurons; i++)
		{
			double h = 0;
			for (int j = 0; j < num_inputs; j++)
			{
				h += input[j] * weights[i][j];
			}
			double output = h > thresholds[i] ? 1.0 : 0.0;
			temp.push_back(output);
		}
		return temp;
	}


	void print_weights()
	{

		
		for (int i = 0; i < num_neurons; i++)
		{
			for (int j = 0; j < num_inputs; j++) {
				std::cout << weights[i][j] << "\t";
			}
			std::cout << thresholds[i] <<std::endl;
		}
	}
};


int main(){
    
    srand(time_t(NULL));
    for(int i = 400 ; i < 7600; i += 400){
        std::ifstream dataset("data/mushrooms.csv");
        std::string line;
        int a;
        std::getline(dataset,line);//ignoring the first line
        Perceptron perceptron(22, 1, 0.01);
        int looper = 0;
        while(std::getline(dataset,line)){
            StringArray strings(line,',');
            std::pair<std::vector<double>, double> row = strings.getNormalizedData();
            perceptron.train(row.first, { row.second });
            if(looper++ == i) break;
    
        }
        double error = 0;
        looper = 0;
        int fp = 0,fn = 0, tp = 0, tn = 0;
        while(std::getline(dataset,line)){
            StringArray strings(line, ',');
            std::pair<std::vector<double>, double> row = strings.getNormalizedData();
            looper++;
            double result = row.second;

            double prediction = perceptron.test(row.first)[0];

            if(result == 0.0 && prediction == 0.0){
                tn++;
            }
            if(result == 0.0 && prediction == 1.0){
                fp++;
            }
            if(result == 1.0 && prediction == 0.0){
                fn++;
            }
            if(result == 1.0 && prediction == 1.0){
                tp++;
            }
            error += fabs(result - prediction);
        }
        std::cout<<"Total Error in "<<looper <<" tests is " <<error <<std::endl;
        std::cout<<"Average Error in "<<looper <<" tests is " <<error/(double)looper <<std::endl;
        std::cout<<"True Positive:\t" << tp <<std::endl;
        std::cout<<"False Positive:\t" << fp <<std::endl;
        std::cout<<"True Negative:\t" << tn <<std::endl;
        std::cout<<"False Negative:\t" << fn <<std::endl;
    }
    return 0;
}
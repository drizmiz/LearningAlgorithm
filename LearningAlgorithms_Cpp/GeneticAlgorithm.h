
#pragma once

#include <string>
#include <vector>
#include <random>

#define STD ::std::

class Constant
{
public:
	static int CITY_NUM; //������
	static constexpr int SPECIES_NUM = 200; //��Ⱥ��
	static constexpr int DEVELOP_NUM = 100; //��������
	static constexpr float pcl = 0.6f, pch = 0.95f;//�������
	static constexpr float pm = 0.4f;//�������
};

class SpeciesNode
{
	STD vector<STD string> genes;//��������
	float distance;//·��
	float fitness;//��Ӧ��
	SpeciesNode* next;
	float rate;

	SpeciesNode() :genes(Constant::CITY_NUM)
	{
		fitness = 0.0f;
		distance = 0.0f;
		next = nullptr;
		rate = 0.0f;
	}

	//��ʼ���ֻ���(���)
	void createByRandomGenes()
	{
		//��ʼ������Ϊ1-CITY_NUM����
		for (int i = 0; i < genes.size(); i++)
		{
			genes[i] = Integer.toString(i + 1);
		}

		//��ȡ�������
		rand = new Random();

		for (int j = 0; j < genes.length; j++)
		{
			int num = j + rand.nextInt(genes.length - j);

			//����
			String tmp;
			tmp = genes[num];
			genes[num] = genes[j];
			genes[j] = tmp;
		}
	}

	//��ʼ���ֻ���(̰��)
	void createByGreedyGenes()
	{
		Random rand = new Random();
		int i = rand.nextInt(Constant.CITY_NUM); //�������һ��������Ϊ���
		genes[0] = Integer.toString(i + 1);
		int j;//�յ�
		int cityNum = 0;
		do
		{
			cityNum++;

			//ѡ����Դ��̳���
			float minDis = Integer.MAX_VALUE;
			int minCity = 0;
			for (j = 0; j < Constant.CITY_NUM; j++)
			{
				if (j != i)
				{
					//���Ƿ�������ظ�
					boolean repeat = false;
					for (int n = 0; n < cityNum; n++)
					{
						if (Integer.parseInt(genes[n]) == j + 1)
						{
							repeat = true;//����
							break;
						}
					}
					if (repeat == false)//û��
					{
						//�г���
						if (Constant.disMap[i][j] < minDis)
						{
							minDis = Constant.disMap[i][j];
							minCity = j;
						}
					}
				}
			}

			//���뵽Ⱦɫ��
			genes[cityNum] = Integer.toString(minCity + 1);
			i = minCity;
		} while (cityNum < Constant.CITY_NUM - 1);
	}

	//����������Ӧ��
	void calFitness()
	{
		float totalDis = 0.0f;
		for (int i = 0; i < Constant.CITY_NUM; i++)
		{
			int curCity = Integer.parseInt(this.genes[i]) - 1;
			int nextCity = Integer.parseInt(this.genes[(i + 1) % Constant.CITY_NUM]) - 1;

			totalDis += Constant.disMap[curCity][nextCity];
		}

		this.distance = totalDis;
		this.fitness = 1.0f / totalDis;
	}

	//���
	public SpeciesNode clone()
	{
		SpeciesNode species = new SpeciesNode();

		//����ֵ
		for (int i = 0; i < this.genes.length; i++)
			species.genes[i] = this.genes[i];
		species.distance = this.distance;
		species.fitness = this.fitness;

		return species;
	}

	//��ӡ·��
	void printRate()
	{
		System.out.print("���·�ߣ�");
		for (int i = 0; i < genes.length; i++)
			System.out.print(genes[i] + "->");
		System.out.print(genes[0] + "\n");
		System.out.print("��̳��ȣ�" + distance);
	}
};
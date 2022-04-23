#include "Flac3DToABAQUS.h"

Flac3DToABAQUS::Flac3DToABAQUS(string inFileName,string outFileName)
{
	this->m_ABAQUSFileName = outFileName;
	this->m_flacFileName = inFileName;
	this->inputFlacData();
	this->outputABAQUSData();
}

bool Flac3DToABAQUS::isNum(string str)
{
     stringstream sin(str);
     double d;
     char c;
     if (!(sin >> d))
     {
         return false;
     }
     if (sin >> c)
     {
         return false;
     }
     return true;
 }

void Flac3DToABAQUS::inputFlacData()
{
	ifstream ifs;
	ifs.open(m_flacFileName, ios::in);
	//Determine whether the file name is wrong
	if (!ifs.is_open())
	{
		cout << "Failed to open this flac file" << endl;
	}

	string m_buf;
	string m_out;
	int m_LineNum = 1;
	int m_ZGROUPLineNum = -1;


	while (getline(ifs, m_buf))
	{
		istringstream record(m_buf);
		
		if (m_buf[0] == 'G')
		{
			int m_Num = 1;

			while (record >> m_out)
			{
				if (m_Num % 6 == 2)
				{
					m_Node.push_back(m_out);
				}
				else if (m_Num % 6 == 3)
				{
					m_NodeXCoordinate.push_back(m_out);
				}
				else if (m_Num % 6 == 4)
				{
					m_NodeYCoordinate.push_back(m_out);
				}
				else if (m_Num % 6 == 5)
				{
					m_NodeZCoordinate.push_back(m_out);
				}
				m_Num+=1;
			}
		}
		else if (m_buf[0] == 'Z')
		{
			if(m_buf[3] == '8')
			{
				int m_Num = 1;
				while (record >> m_out)
				{
					if (m_Num % 12 == 3)
					{
						m_ElementNumB8.push_back(m_out);
					}
					else if (m_Num % 12 == 4 || m_Num % 12 == 5 || m_Num % 12 == 6 || m_Num % 12 == 7 || m_Num % 12 == 8 || m_Num % 12 == 9 || m_Num % 12 == 10 || m_Num % 12 == 11)
					{
						m_ElementNodeNumB8.push_back(m_out);
					}
					m_Num += 1;
				}
			}
			else if (m_buf[3] == '6')
			{
				int m_Num = 1;
				while (record >> m_out)
				{
					if (m_Num % 10 == 3)
					{
						m_ElementNumW6.push_back(m_out);
					}
					else if (m_Num % 10 == 4 || m_Num % 10 == 5 || m_Num % 10 == 6 || m_Num % 10 == 7 || m_Num % 10 == 8 || m_Num % 10 == 9)
					{
						m_ElementNodeNumW6.push_back(m_out);
					}
					m_Num += 1;
				}
			}
			else if (m_buf[3] == '5')
			{
				int m_Num = 1;
				while (record >> m_out)
				{
					if (m_Num % 9 == 3)
					{
						m_ElementNumP5.push_back(m_out);
					}
					else if (m_Num % 9 == 4 || m_Num % 9 == 5 || m_Num % 9 == 6 || m_Num % 9 == 7 || m_Num % 9 == 8)
					{
						m_ElementNodeNumP5.push_back(m_out);
					}
					m_Num += 1;
				}
			}
			else if (m_buf[3] == '4')
			{
				int m_Num = 1;
				while (record >> m_out)
				{
					if (m_Num % 8 == 3)
					{
						m_ElementNumT4.push_back(m_out);
					}
					else if (m_Num % 8 == 4 || m_Num % 8 == 5 || m_Num % 8 == 6 || m_Num % 8 == 7)
					{
						m_ElementNodeNumT4.push_back(m_out);
					}
					m_Num += 1;
				}
			}
			else if (m_buf[1] == 'G')
			{
				int m_Num = 1;
				while (record >> m_out)
				{
					if (m_Num ==2 )
					{
						//Delete ""
						m_out.erase(remove(m_out.begin(), m_out.end(), '\"'), m_out.end());
						m_GroupElements.push_back(m_out);
						m_ZGROUPLineNum = m_LineNum;
						//cout << m_ZGROUPLineNum << endl;
					}
					m_Num += 1;
				}
			}
		}
		else if (m_LineNum == (m_ZGROUPLineNum+1))
		{
			//cout << m_LineNum << endl;
			while (record >> m_out)
			{
				m_GroupElements.push_back(m_out);
			}
			m_ZGROUPLineNum = m_ZGROUPLineNum + 1;
		}
		//cout << m_LineNum << endl;
		//cout << m_ZGROUPLineNum << endl;
	
		m_LineNum = m_LineNum + 1;
	}
}

void Flac3DToABAQUS::outputABAQUSData()
{
	ofstream ofs;
	ofs.open(m_ABAQUSFileName, ios::out);
	ofs << "*Heading" << endl;
	ofs << "*Preprint, echo=NO, model=NO, history=NO, contact=NO" << endl;
	ofs << "*Part, name=Part" << endl;
	ofs << "*Node" << endl;
	for (int i = 0; i < m_Node.size(); i++)
	{
		ofs << m_Node[i] << ", ";
		ofs << m_NodeXCoordinate[i] << ", ";
		ofs << m_NodeYCoordinate[i] << ", ";
		ofs << m_NodeZCoordinate[i] << endl;
	}
	ofs << "*ELEMENT,TYPE=C3D8R,ELSET=solidmap" << endl;
	for (int i = 0; i < m_ElementNumB8.size(); i++)
	{
		ofs << m_ElementNumB8[i];
		for (int j = 0; j < 8 ; j++)
		{
			int index = m_Brick[j];
			ofs << ", "<< m_ElementNodeNumB8[index +i*8];
		}
		ofs << endl;
	}
	ofs << "*ELEMENT,TYPE=C3D8R,ELSET=auto1" << endl;
	for (int i = 0; i < m_ElementNumP5.size(); i++)
	{
		
		ofs << m_ElementNumP5[i];
		for (int j = 0; j < 5; j++)
		{
			int index = m_Pyramid[j];
			if (j != 4)
			{
				ofs << ", " << m_ElementNodeNumP5[index + i * 5];
			}
			else
			{
				ofs << ", " << m_ElementNodeNumP5[index + i * 5];
				ofs << ", " << m_ElementNodeNumP5[index + i * 5];
				ofs << ", " << m_ElementNodeNumP5[index + i * 5];
				ofs << ", " << m_ElementNodeNumP5[index + i * 5];
			}
		}
		ofs << endl;
	}
	ofs << "*ELEMENT,TYPE=C3D6,ELSET=solidmap" << endl;
	for (int i = 0; i < m_ElementNumW6.size(); i++)
	{
		ofs << m_ElementNumW6[i];
		for (int j = 0; j < 6; j++)
		{
			int index = m_Wedge[j];
			ofs << ", " << m_ElementNodeNumW6[index + i * 6];
		}
		ofs << endl;
	}
	ofs << "*ELEMENT,TYPE=C3D4,ELSET=auto1" << endl;
	for (int i = 0; i < m_ElementNumT4.size(); i++)
	{
		ofs << m_ElementNumT4[i];
		for (int j = 0; j < 4; j++)
		{
			int index = m_Tetrahedron[j];
			ofs << ", " << m_ElementNodeNumT4[index + i * 4];
		}
		ofs << endl;
	}
	int m_coumn = 0;
	for (int i = 0; i < m_GroupElements.size(); i++)
	{
		
		if (!isNum(m_GroupElements[i]))
		{
			if (i == 0)
			{
				ofs << "*ELSET, ELSET=" << m_GroupElements[i] << endl;
			}
			else
			{
				ofs << endl;
				ofs << "*ELSET, ELSET=" << m_GroupElements[i] << endl;
			}

			m_coumn = 0;
		}
		else
		{
			m_coumn = m_coumn + 1;
			if (m_coumn % 8 == 0)
			{
				ofs << m_GroupElements[i] << "," << endl;
			}
			else
				ofs << m_GroupElements[i] << ",";
		}





		
	}



}

Flac3DToABAQUS::~Flac3DToABAQUS()
{

}
















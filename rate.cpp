/* �������������ı�ֵ�ĺ���(С�������ϴ����) */
/* ��������������,���ظ������ı�ֵ��� */
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)>(b)?(b):(a))

double rate(double w, double h)
{
	return min(w, h) / max(w, h);
}
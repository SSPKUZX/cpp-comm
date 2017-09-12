#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
	size_t s1 = 0, e1 = nums1.size(), s2 = 0, e2 = nums2.size(), 
		   total_length = e1+e2, m1 = e1/2, m2 = e2/2;
	int32_t k1 = total_length/2, k2 = k1-1;
	auto is_even = (total_length %2 == 0);	
	double median = 0;

	while(true){
		if( nums1[m1] < nums2[m2]){

		}
		m1 = (s1+e1)/2;	
		m2 = (s1+e1)/2;	
	}

	return median;
}

string complexNumberMultiply(string a, string b) {
	auto a_plus_pos = a.find('+');		        
	auto b_plus_pos = b.find('+');		        
	auto a_left		= stoi(a.substr(0, a_plus_pos));
	auto a_right	= stoi(a.substr(a_plus_pos+1, a.length() - a_plus_pos-2));
	auto b_left		= stoi(b.substr(0, b_plus_pos));
	auto b_right	= stoi(b.substr(b_plus_pos+1, b.length() - b_plus_pos-2));

	int left	= a_left*b_left - a_right*b_right;
	int right	= a_left*b_right + b_left*a_right;

	ostringstream oss;
	oss << left << '+' << right << 'i';
	return oss.str(); 
}

int singleNonDuplicate(vector<int>& nums) {
	for( int idx = 0; idx < nums.size()-1; idx+=2){
		if( nums[idx] != nums[idx+1]) return nums[idx];
	}
	return nums[nums.size()-1];
}

int findMin(vector<int>& nums) {
	int s=0, e=nums.size(),	m=e/2;        
	while(s+1<e){ // two or more elements exist
		if(nums[s] < nums[m]){
			if(nums[m]>nums[e-1]){ // [s,m] is in left slope		
				s=m+1;
			} else { //[m,e) is in right slope
				e=m;
			}
		}else if(nums[s] > nums[m]){ // [m,e) is in right slope
			e=m+1;
			++s;
		} else {
			if(nums[e-1]>nums[m]){
				return nums[m];
			}else if( nums[e-1]<nums[m]){
				s=m+1;
			}else{ // nums[s]==nums[m]==nums[e-1]
				while( s+1<e && nums[s]<=nums[s+1]) ++s;
				while( s<e-1 && nums[e-1]==nums[e-2]) --e;
			}
		}
		m=(s+e)/2;
	}
	return nums[s];
}

char findTheDifference(string s, string t) {
	int s_counts[26];	        
	int t_counts[26];	        

	fill(s_counts, s_counts+26, 0);
	fill(t_counts, t_counts+26, 0);
	for(auto c : s) ++s_counts[c-'a'];
	for(auto c : t) ++t_counts[c-'a'];

	for(size_t idx=0; idx<26; ++idx){
		if(s_counts[idx]!=t_counts[idx])
			return 'a'+idx;	
	}
	return 'a';
}

int main()
{
	vector<int> nums1{1};
	vector<int> nums2{2,2,1};
	vector<int> nums3{2, 4, 4, 5,5};

	//cout << findMedianSortedArrays(nums1, nums2)  << '\n';
	cout << complexNumberMultiply("2+3i", "3+4i") << '\n';

	cout <<  singleNonDuplicate( nums1 ) << '\n';
	cout <<  singleNonDuplicate( nums2 )  << '\n';
	cout <<  singleNonDuplicate( nums3 )  << '\n';

	vector<int> mins{4,5,6,7,0,1,2};
	vector<int> mins2{3,4,1,3,3,3};
	vector<int> mins3{1,1,3,3,3};
	vector<int> mins4{1,1,3,3,3};
	vector<int> mins5{3,4,3,3,3};
	vector<int> mins6{1,1,3};
	vector<int> mins7{3,3,1}; 
	vector<int> mins8{3,1,3,3}; // failure
		
	cout << "mins:" << findMin(mins) << '\n';
	cout << "mins:" << findMin(mins2) << '\n';
	cout << "mins:" << findMin(mins3) << '\n';
	cout << "mins:" << findMin(mins4) << '\n';
	cout << "mins:" << findMin(mins5) << '\n';
	cout << "mins:" << findMin(mins6) << '\n';
	cout << "mins:" << findMin(mins7) << '\n';
	cout << "mins:" << findMin(mins8) << '\n';

	cout << boolalpha << findTheDifference("abcd", "abcde") << '\n';
	cout << boolalpha << findTheDifference("ymbgaraibkfmvocpizdydugvalagaivdbfsfbepeyccqfepzvtpyxtbadkhmwmoswrcxnargtlswqemafandgkmydtimuzvjwxvlfwlhvkrgcsithaqlcvrihrwqkpjdhgfgreqoxzfvhjzojhghfwbvpfzectwwhexthbsndovxejsntmjihchaotbgcysfdaojkjldprwyrnischrgmtvjcorypvopfmegizfkvudubnejzfqffvgdoxohuinkyygbdzmshvyqyhsozwvlhevfepdvafgkqpkmcsikfyxczcovrmwqxxbnhfzcjjcpgzjjfateajnnvlbwhyppdleahgaypxidkpwmfqwqyofwdqgxhjaxvyrzupfwesmxbjszolgwqvfiozofncbohduqgiswuiyddmwlwubetyaummenkdfptjczxemryuotrrymrfdxtrebpbjtpnuhsbnovhectpjhfhahbqrfbyxggobsweefcwxpqsspyssrmdhuelkkvyjxswjwofngpwfxvknkjviiavorwyfzlnktmfwxkvwkrwdcxjfzikdyswsuxegmhtnxjraqrdchaauazfhtklxsksbhwgjphgbasfnlwqwukprgvihntsyymdrfovaszjywuqygpvjtvlsvvqbvzsmgweiayhlubnbsitvfxawhfmfiatxvqrcwjshvovxknnxnyyfexqycrlyksderlqarqhkxyaqwlwoqcribumrqjtelhwdvaiysgjlvksrfvjlcaiwrirtkkxbwgicyhvakxgdjwnwmubkiazdjkfmotglclqndqjxethoutvjchjbkoasnnfbgrnycucfpeovruguzumgmgddqwjgdvaujhyqsqtoexmnfuluaqbxoofvotvfoiexbnprrxptchmlctzgqtkivsilwgwgvpidpvasurraqfkcmxhdapjrlrnkbklwkrvoaziznlpor","qhxepbshlrhoecdaodgpousbzfcqjxulatciapuftffahhlmxbufgjuxstfjvljybfxnenlacmjqoymvamphpxnolwijwcecgwbcjhgdybfffwoygikvoecdggplfohemfypxfsvdrseyhmvkoovxhdvoavsqqbrsqrkqhbtmgwaurgisloqjixfwfvwtszcxwktkwesaxsmhsvlitegrlzkvfqoiiwxbzskzoewbkxtphapavbyvhzvgrrfriddnsrftfowhdanvhjvurhljmpxvpddxmzfgwwpkjrfgqptrmumoemhfpojnxzwlrxkcafvbhlwrapubhveattfifsmiounhqusvhywnxhwrgamgnesxmzliyzisqrwvkiyderyotxhwspqrrkeczjysfujvovsfcfouykcqyjoobfdgnlswfzjmyucaxuaslzwfnetekymrwbvponiaojdqnbmboldvvitamntwnyaeppjaohwkrisrlrgwcjqqgxeqerjrbapfzurcwxhcwzugcgnirkkrxdthtbmdqgvqxilllrsbwjhwqszrjtzyetwubdrlyakzxcveufvhqugyawvkivwonvmrgnchkzdysngqdibhkyboyftxcvvjoggecjsajbuqkjjxfvynrjsnvtfvgpgveycxidhhfauvjovmnbqgoxsafknluyimkczykwdgvqwlvvgdmufxdypwnajkncoynqticfetcdafvtqszuwfmrdggifokwmkgzuxnhncmnsstffqpqbplypapctctfhqpihavligbrutxmmygiyaklqtakdidvnvrjfteazeqmbgklrgrorudayokxptswwkcircwuhcavhdparjfkjypkyxhbgwxbkvpvrtzjaetahmxevmkhdfyidhrdeejapfbafwmdqjqszwnwzgclitdhlnkaiyldwkwwzvhyorgbysyjbxsspnjdewjxbhpsvj") << '\n';

	return 0;
}

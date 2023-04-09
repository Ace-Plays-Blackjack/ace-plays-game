#ifndef CARDDISCRIMINATOR
#define CARDDISCRIMINATOR

#include "Card.h"

class CardDiscriminator {
	public:
		CardDiscriminator(cv::String folder_path);
		void template_matching(Card_params &Card_params, bool rank);
		
	private:
	     CardTemplate cardTemplates;
	
};

#endif //CARDDISCRIMINATOR.H

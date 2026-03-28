/**
 * @file tritree.cpp
 * @author CPSC 221
 * @description TriTree class implementation, CPSC 221 PA3
 *
 * This file must be submitted for grading.
 *
**/

#include "tritree.h"

TriTree::TriTree(PNG& imIn) {
	// REPLACE THE LINEs BELOW WITH YOUR CODE
	width = imIn.width();
	height = imIn.height();
	root = BuildNode(imIn, make_pair(0, 0), width, height);
}

void TriTree::Clear() { // private
	Clear_Helper(root);
	root = nullptr;
}

void TriTree::Copy(const TriTree& other) { // private
	width = other.width;
	height = other.height;
	if (other.root == nullptr)
	{
		root = nullptr;
		return;
	}
	else{
	root = Copy_Helper(other.root);
	}
}

PNG TriTree::Render() const {
	PNG newPNG(width, height);
	Render_Helper(newPNG, root);
	return newPNG;
}

void TriTree::Transpose() {
	transpose(root);
	int temp = height;
	height = width;
	width = temp;
}

void TriTree::Prune(double tol) {
	if(root == nullptr){
		return;
	}
	prunehelper(root, tol);	
}

int TriTree::NumLeaves() const {
	return NumLeaves_Helper(root);
}

Node* TriTree::BuildNode(PNG& im, pair<int, int> ul, int w, int h) { // private
	if(w == 1 && h == 1) {
		Node* leaf = new Node(ul, w, h);
		leaf->avg = *im.getPixel(ul.first, ul.second);
		return leaf;
	}	else {
		if (w >= h) {
			int splitedWidth = w / 3;
			int remainder = w % 3;
			if (splitedWidth == 0) {
				Node* A = BuildNode(im, ul, 1, h);
				Node* C = BuildNode(im, make_pair(ul.first + 1,ul.second), 1, h);
				Node* newNode = new Node(ul, w, h);

				int childArea = 1 * h;
				long totalR = A->avg.r * childArea + C->avg.r * childArea;
				long totalG = A->avg.g * childArea + C->avg.g * childArea;
				long totalB = A->avg.b * childArea + C->avg.b * childArea;
				double totolA = A->avg.a * childArea + C->avg.a * childArea;
				newNode->avg.r = (unsigned char)(totalR / (2 * childArea));
				newNode->avg.g = (unsigned char)(totalG / (2 * childArea));
				newNode->avg.b = (unsigned char)(totalB / (2 * childArea));
				newNode->avg.a = totolA / (2 * childArea);

				newNode->A = A;
				newNode->B = nullptr;
				newNode->C = C;
				return newNode;
			}	else if(remainder == 0) {
				Node* A = BuildNode(im, ul, splitedWidth, h);
				Node* B = BuildNode(im, make_pair(ul.first + splitedWidth,ul.second), splitedWidth, h);
				Node* C = BuildNode(im, make_pair(ul.first + 2 * splitedWidth,ul.second), splitedWidth, h);
				Node* newNode = new Node(ul, w, h);

				int childArea = splitedWidth * h;
				long totalR = A->avg.r * childArea + B->avg.r * childArea + C->avg.r * childArea;
				long totalG = A->avg.g * childArea + B->avg.g * childArea + C->avg.g * childArea;
				long totalB = A->avg.b * childArea + B->avg.b * childArea + C->avg.b * childArea;
				double totolA = A->avg.a * childArea + B->avg.a * childArea + C->avg.a * childArea;
				newNode->avg.r = (unsigned char)(totalR / (3 * childArea));
				newNode->avg.g = (unsigned char)(totalG / (3 * childArea));
				newNode->avg.b = (unsigned char)(totalB / (3 * childArea));
				newNode->avg.a = totolA / (3 * childArea);

				newNode->A = A;
				newNode->B = B;
				newNode->C = C;
				return newNode;
			}	else if (remainder == 1) {
				Node* A = BuildNode(im, ul, splitedWidth, h);
				Node* B = BuildNode(im, make_pair(ul.first + splitedWidth,ul.second), splitedWidth + 1, h);
				Node* C = BuildNode(im, make_pair(ul.first + 2 * splitedWidth + 1,ul.second), splitedWidth, h);
				Node* newNode = new Node(ul, w, h);

				int childAreaA = splitedWidth * h;
				int childAreaB = (splitedWidth + 1) * h;
				int childAreaC = splitedWidth * h;
				long totalR = A->avg.r * childAreaA + B->avg.r * childAreaB + C->avg.r * childAreaC;
				long totalG = A->avg.g * childAreaA + B->avg.g * childAreaB + C->avg.g * childAreaC;
				long totalB = A->avg.b * childAreaA + B->avg.b * childAreaB + C->avg.b * childAreaC;
				double totolA = A->avg.a * childAreaA + B->avg.a * childAreaB + C->avg.a * childAreaC;
				newNode->avg.r = (unsigned char)(totalR / (childAreaA + childAreaB + childAreaC));
				newNode->avg.g = (unsigned char)(totalG / (childAreaA + childAreaB + childAreaC));
				newNode->avg.b = (unsigned char)(totalB / (childAreaA + childAreaB + childAreaC));
				newNode->avg.a = totolA / (childAreaA + childAreaB + childAreaC);

				newNode->A = A;
				newNode->B = B;
				newNode->C = C;
				return newNode;
			}	else if (remainder == 2) {
				Node* A = BuildNode(im, ul, splitedWidth + 1, h);
				Node* B = BuildNode(im, make_pair(ul.first + splitedWidth + 1,ul.second), splitedWidth, h);
				Node* C = BuildNode(im, make_pair(ul.first + 2 * splitedWidth + 1,ul.second), splitedWidth + 1, h);
				Node* newNode = new Node(ul, w, h);
				int childAreaA = (splitedWidth + 1) * h;
				int childAreaB = splitedWidth * h;
				int childAreaC = (splitedWidth + 1) * h;
				long totalR = A->avg.r * childAreaA + B->avg.r * childAreaB + C->avg.r * childAreaC;
				long totalG = A->avg.g * childAreaA + B->avg.g * childAreaB + C->avg.g * childAreaC;
				long totalB = A->avg.b * childAreaA + B->avg.b * childAreaB + C->avg.b * childAreaC;
				double totolA = A->avg.a * childAreaA + B->avg.a * childAreaB + C->avg.a * childAreaC;
				newNode->avg.r = (unsigned char)(totalR / (childAreaA + childAreaB + childAreaC));
				newNode->avg.g = (unsigned char)(totalG / (childAreaA + childAreaB + childAreaC));
				newNode->avg.b = (unsigned char)(totalB / (childAreaA + childAreaB + childAreaC));
				newNode->avg.a = totolA / (childAreaA + childAreaB + childAreaC);
				newNode->A = A;
				newNode->B = B;
				newNode->C = C;
				return newNode;
			}
		}	else if (h > w) {
			int splitedHeight = h / 3;
			int remainder = h % 3;
			if (splitedHeight == 0) {
				Node* A = BuildNode(im, ul, w, 1);
				Node* C = BuildNode(im, make_pair(ul.first,ul.second + 1), w, 1);
				Node* newNode = new Node(ul, w, h);

				int childArea = w * 1;
				long totalR = A->avg.r * childArea + C->avg.r * childArea;
				long totalG = A->avg.g * childArea + C->avg.g * childArea;
				long totalB = A->avg.b * childArea + C->avg.b * childArea;
				double totolA = A->avg.a * childArea + C->avg.a * childArea;
				newNode->avg.r = (unsigned char)(totalR / (2 * childArea));
				newNode->avg.g = (unsigned char)(totalG / (2 * childArea));
				newNode->avg.b = (unsigned char)(totalB / (2 * childArea));
				newNode->avg.a = totolA / (2 * childArea);

				newNode->A = A;
				newNode->B = nullptr;
				newNode->C = C;
				return newNode;
			}	else if(remainder == 0) {
				Node* A = BuildNode(im, ul, w, splitedHeight);
				Node* B = BuildNode(im, make_pair(ul.first,ul.second + splitedHeight), w, splitedHeight);
				Node* C = BuildNode(im, make_pair(ul.first,ul.second + 2 * splitedHeight), w, splitedHeight);
				Node* newNode = new Node(ul, w, h);
				int childArea = w * splitedHeight;
				long totalR = A->avg.r * childArea + B->avg.r * childArea + C->avg.r * childArea;
				long totalG = A->avg.g * childArea + B->avg.g * childArea + C->avg.g * childArea;
				long totalB = A->avg.b * childArea + B->avg.b * childArea + C->avg.b * childArea;
				double totolA = A->avg.a * childArea + B->avg.a * childArea + C->avg.a * childArea;
				newNode->avg.r = (unsigned char)(totalR / (3 * childArea));
				newNode->avg.g = (unsigned char)(totalG / (3 * childArea));
				newNode->avg.b = (unsigned char)(totalB / (3 * childArea));
				newNode->avg.a = totolA / (3 * childArea);
				newNode->A = A;
				newNode->B = B;
				newNode->C = C;
				return newNode;
			}	else if (remainder == 1) {
				Node* A = BuildNode(im, ul, w, splitedHeight);
				Node* B = BuildNode(im, make_pair(ul.first,ul.second + splitedHeight), w, splitedHeight + 1);
				Node* C = BuildNode(im, make_pair(ul.first,ul.second + 2 * splitedHeight + 1), w, splitedHeight);
				Node* newNode = new Node(ul, w, h);
				int childAreaA = w * splitedHeight;
				int childAreaB = w * (splitedHeight + 1);
				int childAreaC = w * splitedHeight;
				long totalR = A->avg.r * childAreaA + B->avg.r * childAreaB + C->avg.r * childAreaC;
				long totalG = A->avg.g * childAreaA + B->avg.g * childAreaB + C->avg.g * childAreaC;
				long totalB = A->avg.b * childAreaA + B->avg.b * childAreaB + C->avg.b * childAreaC;
				double totolA = A->avg.a * childAreaA + B->avg.a * childAreaB + C->avg.a * childAreaC;
				newNode->avg.r = (unsigned char)(totalR / (childAreaA + childAreaB + childAreaC));
				newNode->avg.g = (unsigned char)(totalG / (childAreaA + childAreaB + childAreaC));
				newNode->avg.b = (unsigned char)(totalB / (childAreaA + childAreaB + childAreaC));
				newNode->avg.a = totolA / (childAreaA + childAreaB + childAreaC);
				newNode->A = A;
				newNode->B = B;
				newNode->C = C;
				return newNode;
			}	else if (remainder == 2) {
				Node* A = BuildNode(im, ul, w, splitedHeight + 1);
				Node* B = BuildNode(im, make_pair(ul.first,ul.second + splitedHeight + 1), w, splitedHeight);
				Node* C = BuildNode(im, make_pair(ul.first,ul.second + 2 * splitedHeight + 1), w, splitedHeight + 1);
				Node* newNode = new Node(ul, w, h);
				int childAreaA = w * (splitedHeight + 1);
				int childAreaB = w * splitedHeight;
				int childAreaC = w * (splitedHeight + 1);
				long totalR = A->avg.r * childAreaA + B->avg.r * childAreaB + C->avg.r * childAreaC;
				long totalG = A->avg.g * childAreaA + B->avg.g * childAreaB + C->avg.g * childAreaC;
				long totalB = A->avg.b * childAreaA + B->avg.b * childAreaB + C->avg.b * childAreaC;
				double totolA = A->avg.a * childAreaA + B->avg.a * childAreaB + C->avg.a * childAreaC;
				newNode->avg.r = (unsigned char)(totalR / (childAreaA + childAreaB + childAreaC));
				newNode->avg.g = (unsigned char)(totalG / (childAreaA + childAreaB + childAreaC));
				newNode->avg.b = (unsigned char)(totalB / (childAreaA + childAreaB + childAreaC));
				newNode->avg.a = totolA / (childAreaA + childAreaB + childAreaC);
				newNode->A = A;
				newNode->B = B;
				newNode->C = C;
				return newNode;
			}
		}
	}
	return nullptr; // should never reach here
}

/*==== ALSO IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE DECLARED ====*/
int TriTree::NumLeaves_Helper(Node* nd) const {
	if (nd == nullptr) {
		return 0;
	}
	if (nd->A == nullptr && nd->B == nullptr && nd->C == nullptr) {
		return 1;
	}
	return NumLeaves_Helper(nd->A) + NumLeaves_Helper(nd->B) + NumLeaves_Helper(nd->C);
}

void TriTree::Clear_Helper(Node* nd) {
	if (nd == nullptr) {
		return;
	}
	Clear_Helper(nd->A);
	Clear_Helper(nd->B);
	Clear_Helper(nd->C);
	delete nd;
}

Node* TriTree::Copy_Helper(const Node* otherNode) {
	if (otherNode == nullptr) {
		return nullptr;
	}
	Node* newNode = new Node(otherNode->upperleft, otherNode->width, otherNode->height);
	newNode->avg = otherNode->avg;
	newNode->A = Copy_Helper(otherNode->A);
	newNode->B = Copy_Helper(otherNode->B);
	newNode->C = Copy_Helper(otherNode->C);
	return newNode;
}

void TriTree::Render_Helper(PNG& png, const Node* nd) const {
	if(nd == nullptr) {
		return;
	}
	if (nd->A == nullptr && nd->B == nullptr && nd->C == nullptr) {
		for (int i = 0; i < nd->width; i++) {
			for (int j = 0; j < nd->height; j++) {
				*png.getPixel(nd->upperleft.first + i, nd->upperleft.second + j) = nd->avg;
			}
		}
	} else {
		Render_Helper(png, nd->A);
		Render_Helper(png, nd->B);
		Render_Helper(png, nd->C);
	}
}

void TriTree::transpose(Node* root){
	if(root == nullptr){
		return;
	}
	int upperleftX = root->upperleft.first;
	int upperleftY = root->upperleft.second;
	root->upperleft.first = upperleftY;
	root->upperleft.second = upperleftX;
	int temp = root->height;
	root->height = root->width;
	root->width = temp;
	transpose(root->A);
	transpose(root->B);
	transpose(root->C);
}

bool TriTree::canPrune(const Node* root, const RGBAPixel& avg, double tol) const {
	if (root == nullptr) {
		return true;
	}
	if (root->A == nullptr && root->B == nullptr && root->C == nullptr) {
		return avg.dist(root->avg) <= tol;
	}
	return canPrune(root->A, avg, tol) && canPrune(root->B, avg, tol) && canPrune(root->C, avg, tol);
}

void  TriTree::prunehelper(Node* root, double tol) {
	if (root == nullptr) {
		return;
	}
    if (root->A == nullptr && root->B == nullptr && root->C == nullptr) {
        return;
    }
	
	if (canPrune(root, root->avg, tol)) {
		Clear_Helper(root->A);
		Clear_Helper(root->B);
		Clear_Helper(root->C);
		root->A = nullptr;
		root->B = nullptr;
		root->C = nullptr;
	} else {
		prunehelper(root->A, tol);
		prunehelper(root->B, tol);
		prunehelper(root->C, tol);
	}
}
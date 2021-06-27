#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofDisableArbTex();

	this->cap_size = cv::Size(640, 360);
	this->rect_size = 10;
	for (int x = 0; x < this->cap_size.width; x += this->rect_size) {

		for (int y = 0; y < this->cap_size.height; y += this->rect_size) {

			auto image = make_unique<ofImage>();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);

			this->rect_image_list.push_back(move(image));
			this->cv_rect_list.push_back(rect);
			this->rect_frame_list.push_back(frame);
			this->draw_rect_size_list.push_back(this->rect_size);
		}
	}

	auto file_path = "D:\\MP4\\Pexels Videos 2740.mp4";
	this->cap.open(file_path);
	this->number_of_frames = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			continue;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int frame_index = (int)(ofGetFrameNum() * 0.5) % this->number_of_frames;
	int next_frame_index = (frame_index + 1) % this->number_of_frames;

	for (int i = 0; i < this->draw_rect_size_list.size(); i++) {

		this->draw_rect_size_list[i] = this->draw_rect_size_list[i] <= 0 ? 0 : this->draw_rect_size_list[i] - 0.3;

		if (ofNoise(this->cv_rect_list[i].x * 0.01, this->cv_rect_list[i].y * 0.01, ofGetFrameNum() * 0.01) < 0.5) {

			this->draw_rect_size_list[i] = this->rect_size;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	ofFill();

	for (int i = 0; i < this->rect_frame_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->cv_rect_list[i].x + 50 - ofGetWidth() * 0.5, this->cv_rect_list[i].y + 180 - ofGetHeight() * 0.5, -1);
		ofRotateX(180);

		int frame_index = (int)(ofGetFrameNum() * 0.42) % this->number_of_frames;

		cv::Mat tmp_box_image(this->frame_list[frame_index], this->cv_rect_list[i]);
		tmp_box_image.copyTo(this->rect_frame_list[i]);

		if (this->draw_rect_size_list[i] > 0) {

			this->rect_image_list[i]->update();
			this->rect_image_list[i]->getTexture().bind();

			auto height = this->draw_rect_size_list[i] * 5;

			ofSetColor(ofMap(this->draw_rect_size_list[i], 0, this->rect_size, 0, 255));
			ofDrawBox(glm::vec3(), this->rect_size);
			this->rect_image_list[i]->unbind();
		}

		ofPopMatrix();
	}

	ofNoFill();
	ofSetColor(39);
	ofDrawBox(glm::vec3(this->rect_size * 0.5, this->rect_size * -0.5, 0), 640, 360, this->rect_size);

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
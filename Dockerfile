FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  git \
  ghostscript \
  curl \
  pkg-config \
  zip \
  unzip \
  tar \
  && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN git clone https://github.com/Microsoft/vcpkg.git && \
  ./vcpkg/bootstrap-vcpkg.sh && \
  mkdir -p build && cd build && \
  cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake && \
  make
EXPOSE 8080
CMD ["./build/revisepdf"]


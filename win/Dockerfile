FROM grangerhub/tremulous13:latest
RUN curl https://sh.rustup.rs -sSf | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"
RUN cargo install reserved; exit 0
RUN rustup target add x86_64-pc-windows-gnu
RUN apt-get update && apt-get install -y libclang-dev clang llvm mingw-w64 && rm -rf /var/lib/apt/lists/*
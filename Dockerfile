FROM buildpack-deps:stretch
MAINTAINER github@digressed.net

ENV LC_ALL C.UTF-8

# Update Ubuntu Software repository
RUN apt-get update

# install dependencies & utils
RUN apt-get install -y \
    libicu-dev \
    curl \
    libreadline-dev 

# aliases, environment
RUN echo 'alias h="history"' >> /root/.bashrc
RUN echo 'alias be="bundle exec"' >> /root/.bashrc
ENV PATH "/root/.rbenv/bin:/root/.rbenv/shims:$PATH"

# install rbenv & ruby
RUN \curl -sL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-installer | bash -
RUN eval "$(rbenv init -)"

ENV RUBY_VERSION=2.6.3
RUN rbenv install ${RUBY_VERSION}
RUN rbenv global ${RUBY_VERSION}

# mute bundle warning about root user
RUN bundle config --global silence_root_warning 1

WORKDIR /gem

# cleanup
RUN rm -rf /var/lib/apt/lists/*

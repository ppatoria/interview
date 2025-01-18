       push   %r12
       pxor   %xmm0,%xmm0
       mov    %rdi,%r12
       push   %rbp
       push   %rbx
       sub    $0x20,%rsp
       mov    %rsp,%rdi
       movaps %xmm0,(%rsp)
       movq   $0x0,0x10(%rsp)
       callq  409490 <void populateSimulatedInput<std::vector<MarketDataAlligned, aligned_allocator<MarketDataAlligned, 64ul> > >(std::vector<MarketDataAlligned, aligned_allocator<MarketDataAlligned, 64ul> >&)>
       cmpb   $0x0,0x1a(%r12)
       je     408f0b <ProcessAlignedMarketData(benchmark::State&)+0xdb>
       mov    %r12,%rdi
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    (%rsp),%rbx
       nopl   0x0(%rax)
       mov    %r12,%rdi
       callq  40bfa0 <benchmark::State::FinishKeepRunning()>
       mov    $0xa,%edx
       mov    $0x42501e,%esi
       mov    $0x42e640,%edi
       callq  4054f0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
       mov    0x8(%rsp),%rsi
       mov    $0x42e640,%edi
       sub    %rbx,%rsi
       sar    $0x5,%rsi
       callq  405300 <std::ostream& std::ostream::_M_insert<unsigned long>(unsigned long)@plt>
       mov    $0x15,%edx
       mov    $0x425029,%esi
       mov    %rax,%rdi
       mov    %rax,%rbp
       callq  4054f0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
       mov    0x0(%rbp),%rax
       mov    -0x18(%rax),%rax
       mov    0xf0(%rbp,%rax,1),%r12
       test   %r12,%r12
       je     408f8f <ProcessAlignedMarketData(benchmark::State&)+0x15f>
       cmpb   $0x0,0x38(%r12)
       je     408f58 <ProcessAlignedMarketData(benchmark::State&)+0x128>
       movsbl 0x43(%r12),%esi
       mov    %rbp,%rdi
       callq  405040 <std::ostream::put(char)@plt>
       mov    %rax,%rdi
       callq  405310 <std::ostream::flush()@plt>
       test   %rbx,%rbx
       je     408f86 <ProcessAlignedMarketData(benchmark::State&)+0x156>
       add    $0x20,%rsp
       mov    %rbx,%rdi
       pop    %rbx
       pop    %rbp
       pop    %r12
       jmpq   405560 <free@plt>
       mov    %r12,%rdi
       mov    0x10(%r12),%rbp
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    (%rsp),%rbx
       test   %rbp,%rbp
       je     408e70 <ProcessAlignedMarketData(benchmark::State&)+0x40>
       nopl   (%rax)
       mov    0x8(%rsp),%rdx
       cmp    %rbx,%rdx
       je     408e70 <ProcessAlignedMarketData(benchmark::State&)+0x40>
       mov    %rbx,%rax
       nopl   0x0(%rax)
99.85% add    $0x20,%rax
0.15%  cmp    %rax,%rdx
       jne    408f40 <ProcessAlignedMarketData(benchmark::State&)+0x110>
       mov    (%rsp),%rbx
       sub    $0x1,%rbp
       jne    408f28 <ProcessAlignedMarketData(benchmark::State&)+0xf8>
       jmpq   408e70 <ProcessAlignedMarketData(benchmark::State&)+0x40>
       mov    %r12,%rdi
       callq  405500 <std::ctype<char>::_M_widen_init() const@plt>
       mov    (%r12),%rax
       mov    $0xa,%esi
       mov    0x30(%rax),%rax
       cmp    $0x4092b0,%rax
       je     408ee2 <ProcessAlignedMarketData(benchmark::State&)+0xb2>
       mov    %r12,%rdi
       callq  *%rax
       movsbl %al,%esi
       jmpq   408ee2 <ProcessAlignedMarketData(benchmark::State&)+0xb2>
       add    $0x20,%rsp
       pop    %rbx
       pop    %rbp
       pop    %r12
       retq
       callq  4055c0 <std::__throw_bad_cast()@plt>
       mov    %rax,%rbx
       jmpq   405950 <ProcessAlignedMarketData(benchmark::State&) [clone .cold]>

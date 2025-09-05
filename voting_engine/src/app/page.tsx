'use client'

import Image from "next/image";
import Link from "next/link";
import { useRouter } from "next/navigation";

export default function Home() {
  
  const router = useRouter()

  return (
    <div className="font-sans grid grid-rows-[20px_1fr_20px] items-center justify-items-center min-h-screen p-8 pb-20 gap-16 sm:p-20">
      <main className="flex flex-col gap-[32px] row-start-2 items-center sm:items-start">
        <Image
          className="dark:invert"
          src="/img/person.svg"
          alt="Next.js logo"
          width={180}
          height={38}
          priority
        />
        
        <ol className="font-mono list-inside list-decimal">
        <li>
          Voting or some thing
        </li>
        <li>
          Best cat
        </li>
        <li>
          Any dog
        </li>
        </ol>

        <div className="flex gap-4 items-center flex-col sm:flex-row">
          {/* <a
            className="rounded-full border border-solid border-transparent transition-colors flex items-center justify-center bg-foreground text-background gap-2 hover:bg-[#383838] dark:hover:bg-[#ccc] font-medium text-sm sm:text-base h-10 sm:h-12 px-4 sm:px-5 sm:w-auto"
            href="https://vercel.com/new?utm_source=create-next-app&utm_medium=appdir-template-tw&utm_campaign=create-next-app"
            target="_blank"
            rel="noopener noreferrer"
          >

            <Image
              className="dark:invert"
              src="/vercel.svg"
              alt="Vercel logomark"
              width={20}
              height={20}
            />
            Deploy now
          </a> */}

          <Link 
            className="rounded-full border border-solid border-black/[.08] dark:border-white/[.145]"
            href="/host"
            target=""
            rel="noopener noreferrer"
          >
            Host a vote
          </Link>

          <a
            className="rounded-full border border-solid border-black/[.08] dark:border-white/[.145]"
            href="/join"
            target=""
            rel="noopener noreferrer"
          >
            Join a vote
          </a>
        </div>
      </main>


      <footer className="row-start-3 flex gap-[24px] flex-wrap items-center justify-center">
        Footer
      </footer>
    </div>
  );
}
